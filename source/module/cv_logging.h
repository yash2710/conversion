#ifndef CV_LOGGING_H
#define CV_LOGGING_H

#include "cv_merge.h"

struct ksnap;
struct ksnap_user_data;
struct cv_logging_page_status_entry;

//every N committed pages, we check to see if this page should be turned into a logging page
//#define CV_LOGGING_DIFF_CHECK_COMMITTED_PAGES 64

#define CV_LOGGING_DIFF_CHECK_COMMITTED_PAGES 1

//how many 64bit words can be different in order to trigger a switch to logging
#define CV_LOGGING_DIFF_THRESHOLD_64 1

#define CV_LOGGING_SWITCH_BITMASK 0xFUL

//size we log in bytes
#define CV_LOGGING_LOG_SIZE 64UL

#define CV_LOGGING_LOG_MASK (~((1<<6)-1))

#define CV_LOGGING_MERGE_WORDS (CV_LOGGING_LOG_SIZE/sizeof(uint64_t))

#define cv_logging_should_switch(bits) ((bits & CV_LOGGING_SWITCH_BITMASK)==CV_LOGGING_SWITCH_BITMASK)

#define cv_logging_is_full_page(e) (e->data_len>=PAGE_SIZE)

#define cv_logging_line_start(addr) (addr & CV_LOGGING_LOG_MASK )

int cv_logging_diff_64(uint8_t * local, struct page * ref_page);

void cv_merge_lines(uint8_t * local, uint8_t * latest, uint8_t * ref);

void cv_logging_print_stats(struct ksnap * cv_seg);

void cv_logging_instruction_stats(struct ksnap * cv_seg, struct ksnap_user_data * cv_user, unsigned long addr);

int cv_logging_fault(struct vm_area_struct * vma, struct ksnap * cv_seg, struct ksnap_user_data * cv_user,
                     struct pt_regs * regs, unsigned long faulting_addr);

unsigned long cv_logging_get_index(unsigned long addr, unsigned long page_index, int is_page_level);

/*functions that work with the page_status_entry data structure. That keeps track of each thread's local logging pages*/

int cv_logging_page_status_insert(struct ksnap_user_data * cv_user, struct cv_logging_page_status_entry * entry, unsigned long index);

struct cv_logging_page_status_entry * cv_logging_page_status_lookup(struct ksnap_user_data * cv_user, unsigned long index);

struct cv_logging_page_status_entry * cv_logging_page_status_entry_init(pte_t * pte, unsigned long pfn);

void cv_merge_line(uint8_t * local, uint8_t * ref, uint8_t * latest);


#endif