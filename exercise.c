#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



enum {SPED = 256, PED = 512, ABS = 1024, TIRE_P = 2048};

struct Frame {
    uint32_t msg_id;
	uint32_t timestamp;
	uint32_t ucm_id;
	uint32_t value1;
	uint32_t value2;
};

struct FrameNode {
	struct FrameNode * next;
	struct Frame * frame;
    int anomally;
};

struct SummaryFrame {
    uint32_t speed;
    uint32_t speed_msg_id;
    uint32_t pressure;
    uint32_t preasure_msg_id;
    uint32_t abs;
    uint32_t abs_msg_id;
    uint32_t pedal_accelerate;
    uint32_t pedal_break;
    uint32_t pedal_msg_id;
} ;

struct SummaryFrameNode {
    struct SummaryFrameNode * next;
    struct SummaryFrame frame;
    uint32_t anomally_msg_id;
};


void frames_free(struct FrameNode *pNode) {

        struct FrameNode * tmp;
        while (pNode != NULL){
            tmp = pNode;
            pNode =pNode->next;
            free(tmp);
        }
        free(pNode);
}

void summary_free(struct SummaryFrameNode *pNode) {
    struct SummaryFrameNode * tmp;
    while (pNode != NULL){
        tmp = pNode;
        pNode =pNode->next;
        free(tmp);
    }
    free(pNode);
}


//check for frequency anomalies

void inspect_frequency_of_one(struct FrameNode * start, struct FrameNode * end, uint32_t ucm_id)
{
    struct FrameNode * current = start;
    int matched = 0;

    while(current != end->next)
    {

        if(current->frame->ucm_id == ucm_id)
        {
            if(matched) {
                current->anomally = 1;
            } else {
                matched = 1;
            }
        }

        current = current -> next;
    }
}


void inspect_speedometer_frequency (struct FrameNode * start, struct FrameNode * end)
{
    inspect_frequency_of_one(start, end, SPED);
}

void inspect_pedals_frequency(struct FrameNode * start, struct FrameNode * end)
{
    inspect_frequency_of_one(start, end, PED);
}

void inspect_abs_frequency (struct FrameNode * start, struct FrameNode * end)
{
    inspect_frequency_of_one(start, end, ABS);
}

void inspect_tire_pressure_frequency (struct FrameNode * start, struct FrameNode * end)
{
    inspect_frequency_of_one(start, end, TIRE_P);
}

typedef void(* check_anomalies_func)(struct FrameNode * window_start, struct FrameNode * window_end);

//create window of each frequency
void check_window(struct FrameNode * frame_start, int window_size, check_anomalies_func anomaly_detector) {
    struct FrameNode * window_start = frame_start;
    struct FrameNode * window_end = frame_start;

    do {
        // Move end pointer
        while ((window_end->next != NULL) &&
               ((window_end->next->frame->timestamp - window_start->frame->timestamp) < window_size)) {
            window_end = window_end->next;
        }


        // Check anomalies
        anomaly_detector(window_start, window_end);

        // Move start pointer
        while ((window_start->next != NULL) &&
               (window_start->frame->timestamp == window_start->next->frame->timestamp)) {
            window_start = window_start->next;
        }

        if (window_start->next == NULL) {
            break;
        }

        window_start = window_start->next;
    } while (window_end->next != NULL);
}


// read input into list
struct FrameNode * frames_read(const char *file_path) {
    struct Frame fr;
	struct FrameNode root;
	struct FrameNode * parent = &root;
    char buffer[100] = {0};
    FILE *fp  = fopen(file_path, "r");
    while (fgets(buffer, 100 , fp) != NULL) {

		struct FrameNode * node = NULL;

        sscanf(buffer,"%d %d %x %d %d ", &(fr.msg_id), &(fr.timestamp), &(fr.ucm_id), &(fr.value1), &(fr.value2));

		node = (struct FrameNode *)malloc(sizeof(struct FrameNode));
		node->frame = (struct Frame *)malloc(sizeof(struct Frame));

		node->frame->msg_id = fr.msg_id ;
		node->frame->timestamp = fr.timestamp;
        node->frame->ucm_id = fr.ucm_id;
        node->frame->value1 = fr.value1;
        node->frame->value2 = fr.value2;

		parent->next = node;
		parent = node;
	}
	return root.next;
}


int detect_timing_anomalies(const char* file_path, unsigned int *anomalies_ids)
{
	// Read input
	size_t anomally_id_next = 0;
	struct FrameNode * frames = frames_read(file_path);
    struct FrameNode *subset_start = frames;


    check_window(frames, 5, inspect_pedals_frequency);
    check_window(frames, 50, inspect_speedometer_frequency);
    check_window(frames, 100, inspect_tire_pressure_frequency);
    check_window(frames, 10, inspect_abs_frequency);


    // Go over the list and add the ids of the anomaliers


    struct FrameNode * head = subset_start;
    while(head != NULL)
    {
        if(head -> anomally == 1)
        {
            anomalies_ids[anomally_id_next] = head -> frame -> msg_id;
            anomally_id_next++;
        }
        if (anomally_id_next >= 1000) {
            break;
        }

        head = head ->next;
    }
    for(size_t i = 0; i < anomally_id_next;i++){
        printf("%d\n", anomalies_ids[i]);
    }

   
	// Free memory
	frames_free(frames);

	return 0;
}

/* behavioral anomalies */

//check range
void one_frame_time(struct FrameNode * head)
{
    struct FrameNode * current = head;

    while(current != NULL)
    {

        if(current -> frame->ucm_id == SPED )
        {
            if(current -> frame->value2 < 0 || current -> frame->value2 > 300)
            {
                current->anomally = 1;
            }
        }
        if(current -> frame->ucm_id == PED )
        {
            if (current->frame->value1 < 0 || current->frame->value1 > 100) {
                current->anomally = 1;
            } else if (current->frame->value2 < 0 || current->frame->value2 > 100) {
                current->anomally = 1;
            } else if (current->frame->value1 > 0 && current->frame->value2 > 0) {
                current->anomally = 1;
            }
        }
        if(current -> frame->ucm_id == ABS)
        {
            if (current -> frame->value1 < 0 || current -> frame->value1 > 1 ) {
                current->anomally = 1;
            }
            if (current -> frame->value2 < 0 || current -> frame->value2 > 1 ) {
                current->anomally = 1;
            }
        }
        if(current -> frame->ucm_id == TIRE_P)
        {

            if(current-> frame->value2 < 0 || current->frame->value2 > 100)
            {
                current->anomally = 1;
            }
        }
        current = current -> next;
    }

}

//pedals behavior anomalies
void behavioral_anomaly_pedals(struct FrameNode * start,struct FrameNode * end){

    if(end->frame->timestamp - start->frame->timestamp < 10){
        end->anomally=1;
    }
}

void speed_pedal_window (struct FrameNode * start) {
    struct FrameNode * first = start;
    struct FrameNode * current = first;

    while(current != NULL){

        if((first->frame->ucm_id == PED && first->frame->value1 == 0) || first->frame->ucm_id != PED  ){
            first = first->next;
        }
        if (first->next == NULL) {
            break;
        }

        if(first->frame->ucm_id==PED && first->frame->value1 > 0){

            while(current->next != NULL){
                if(current->next->frame->ucm_id==PED && current->next->frame->value1 ==0) {
                    current = current->next;
                    break;

                }else {
                    current = current->next;
                }
            }
            behavioral_anomaly_pedals(first,current);
            first = current->next;
        }
        current=current->next;
    }
}

void break_pedal_window (struct FrameNode * start) {
    struct FrameNode * first = start;
    struct FrameNode * current = first;

    while(current != NULL){

        if((first->frame->ucm_id == PED && first->frame->value2 == 0) || first->frame->ucm_id != PED  ){
            first = first->next;
        }
        if (first->next == NULL) {
            break;
        }
        if(first->frame->ucm_id==PED && first->frame->value2 > 0){

            while(current->next != NULL){
                if(current->next->frame->ucm_id==PED && current->next->frame->value2 ==0) {
                    current = current->next;
                    break;

                }else {
                    current = current->next;
                }
            }
            behavioral_anomaly_pedals(first,current);
            first = current->next;
        }
        current=current->next;
    }
}


//check speed anomalies
void inspect_behavioral_anomalies(struct FrameNode * start, struct FrameNode * end)
{
    struct FrameNode * current = end;
    struct FrameNode * first = start;
    struct FrameNode * check = start;



    if(first->frame->value2 == 0){
        if(current->frame->value2 - first->frame->value2 > 5){
            current->anomally =1;
        }
    }

    if(first->frame->value2 <= 5 && current->frame->value2 == 0){

    }

    if(first->frame->value2 >= 0 && current->frame->value2 != 0){
        if(current->frame->value2 - first->frame->value2 > 5 && current->frame->value2 - first->frame->value2 < -5 ){
            current->anomally=1;
        }
    }

    if(current->frame->value2==0 && first->frame->value2 > 5) {
        check = current;
        while(check != NULL){
            if(check->frame->ucm_id == SPED && check->frame->value2 > 0){
                check->anomally=1;
            }
            check = check ->next;
        }
    }
}

//check 50 ms window for speed behavior anomalies and car crash.
void check_window_behavioral(struct FrameNode * frame_start) {
    struct FrameNode * first = frame_start;
    struct FrameNode * current = frame_start;

    while(current != NULL){

        if(first->frame->value2 <= 5 && first->frame->ucm_id == SPED){

            if(current->frame->ucm_id == SPED && current->frame->value2 == 0) {

                first = first->next;
            }
        }
        if(current->frame->ucm_id == SPED && current->frame->msg_id != first->frame->msg_id  ){
            // Check anomalies
            inspect_behavioral_anomalies(first,current);
            first = current;
        }
        current = current->next;
    }
}


int detect_behavioral_anomalies(const char* file_path, unsigned int *anomalies_ids)
{
	// Read input
	int anomally_id_next = 0;
	struct FrameNode * frames = frames_read(file_path);
    struct FrameNode * subset_start = frames;


    struct FrameNode * window_start = frames;


    one_frame_time(window_start);
    speed_pedal_window (window_start);
    break_pedal_window(window_start);
    check_window_behavioral(window_start);



    struct FrameNode * head = subset_start;
    while(head != NULL)
    {
        if(head -> anomally == 1)
        {
            anomalies_ids[anomally_id_next] = head -> frame -> msg_id;
            anomally_id_next++;
        }
        if (anomally_id_next >= 1000) {
            break;
        }

        head = head ->next;
    }


    // Free memory
	frames_free(frames);

	return 0;
}

/* corrolation anomalies
 * creation of a new list
 * combining all the data from the same timestamp
 */
struct SummaryFrameNode * frames_summarize(struct FrameNode * frames) {
    struct FrameNode * window_start = frames;
    struct FrameNode * window_end = frames;
    struct FrameNode * window_pointer = NULL;
    struct SummaryFrameNode summaries = {0};
    struct SummaryFrameNode * summary_parent = &summaries;

    while (NULL != window_start) {
        // Find end of window
        while ((NULL != window_end->next) && (window_end->next->frame->timestamp == window_start->frame->timestamp)) {
            window_end = window_end->next;
        }

        // Summarize window into a row
        struct SummaryFrameNode * summary_new = (struct SummaryFrameNode *)malloc(sizeof(struct SummaryFrameNode));
        summary_new->frame = summary_parent->frame;

        window_pointer = window_start;
        do {
            switch (window_pointer->frame->ucm_id) {
                case SPED:
                    summary_new->frame.speed =window_pointer->frame->value2;
                    summary_new->frame.speed_msg_id = window_pointer->frame->msg_id;
                    break;

                case PED:
                    summary_new->frame.pedal_accelerate = window_pointer->frame->value1;
                    summary_new->frame.pedal_break = window_pointer->frame->value2;
                    summary_new->frame.pedal_msg_id = window_pointer->frame->msg_id;
                    break;

                case ABS:
                    summary_new->frame.abs = window_pointer->frame->value2;
                    summary_new->frame.abs_msg_id = window_pointer->frame->msg_id;
                    break;

                case TIRE_P:
                    summary_new->frame.pressure = window_pointer->frame->value2;
                    summary_new->frame.preasure_msg_id = window_pointer->frame->msg_id;
                    break;
            }

            if (window_pointer == window_end) {
                break;
            }
            window_pointer = window_pointer->next;
        } while (1);

        summary_parent->next = summary_new;
        summary_parent = summary_new;

        // Move next window
        window_start = window_end->next;
        window_end = window_end->next;
    }

    return summaries.next;
}


/* the Function works partially */

void check_behavioral_anomalies(struct SummaryFrameNode * first, struct SummaryFrameNode * second) {
    if (NULL != second) {

        if ((first->frame.pedal_accelerate) && (second->frame.pedal_accelerate >= first->frame.pedal_accelerate) &&
            first->frame.speed && (second->frame.speed < first->frame.speed)) {
            second->anomally_msg_id = second->frame.speed_msg_id;
        }

        if (first->frame.pressure && (second->frame.pressure > first->frame.pressure) && second->frame.speed ) {
            second->anomally_msg_id = second->frame.preasure_msg_id;

        }

        if (second->frame.pedal_break && (second->frame.speed > first->frame.speed)) {
            second->anomally_msg_id = second->frame.speed_msg_id;
        }

        if((first->frame.preasure_msg_id < first->frame.speed_msg_id) && (first->frame.pressure < 30) && (second->frame.speed > 50) ){
            second->anomally_msg_id = second->frame.speed_msg_id;

        }
        if(first->frame.abs_msg_id > first->frame.pedal_msg_id){
            if(first->frame.pedal_break >= 80 &&  (first->frame.abs == 0 || second->frame.abs == 0)){
                first->anomally_msg_id = first->frame.abs_msg_id;
            }
        }

    }


}

int detect_correlation_anomalies(const char* file_path, unsigned int *anomalies_ids)
{
    size_t anomally_id_next = 0;
    struct FrameNode * frames = frames_read(file_path);
    struct FrameNode *subset_start = frames;


    // Summary all frame into summary frames
    struct SummaryFrameNode * summary_frames = frames_summarize(frames);

    // Check anomalies on summary frames
    struct SummaryFrameNode * summary_pointer = summary_frames;

    while (summary_pointer != NULL) {

        check_behavioral_anomalies(summary_pointer, summary_pointer->next);

        summary_pointer = summary_pointer->next;
    }

    struct SummaryFrameNode * head = summary_frames;
    while(head != NULL)
    {
        if(head -> anomally_msg_id != 0)
        {
            anomalies_ids[anomally_id_next] = head -> anomally_msg_id;
            anomally_id_next++;
        }
        if (anomally_id_next >= 1000) {
            break;
        }

        head = head ->next;
    }

    // Free memory
	frames_free(frames);
    summary_free(summary_frames);
	return 0;
}

