# include "os.h"
# include <stdio.h>

/* references :
1. https://www.youtube.com/watch?v=R-WgaQQHSPw&t=303s 
2. https://www.youtube.com/watch?v=NKD_VI4vZp4&t=674s
4. https://www.geeksforgeeks.org/multilevel-paging-in-operating-system/
3. gpt : 
3.1 what is uint64 : https://chatgpt.com/share/674b59bd-d98c-8002-8ad2-c6ad3d154b30
3.2 https://chatgpt.com/share/674b5cd5-d324-8002-ad55-0c7757604ae4
*/
void page_table_update(uint64_t pt, uint64_t vpn , uint64_t ppn) 
{
    uint64_t pt_cos = pt<<12 ; /* add 0 offset*/
    uint64_t *root_ptr = phys_to_virt(pt_cos); /*get a pointer to virt address of pagetable root */
    uint64_t key ; 
    for (int i =4 ; i>=0 ; i--) /* the vpn breaks to 5 , so we loop 5 times */
    {
        key = (vpn>>9*i) & 0x1FF; 
        /*we assume that each table is uint64_t array , each have 512 entries.
        use 1 maks to get the msb , check if result equals zero , i.e its not valid
        */
        if(i==0) /*it means we at final level*/
        {
            if (ppn == NO_MAPPING) root_ptr[key] = 0 ;/* set valid bit to zero*/

            else 
            {
                /* now pte points to ppn , and we completed the mapping*/
                root_ptr[key] = (ppn<<12) +1 ; /* +1 means we turn valid bit on*/
            }
            return ; /* finished the mapping , no need to continue*/
        }

        if ((root_ptr[key] & 1) ==0) 
        {
            if(ppn == NO_MAPPING) {
                return ;
            }
            uint64_t new_frame = alloc_page_frame();
            root_ptr[key] = (new_frame<<12) + 1 ;/*now it points to the new frame we allocated*/
        }

        root_ptr = phys_to_virt(root_ptr[key]-1); 
    }
}


uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
        uint64_t *ptr ;
        uint64_t  pt1 = pt<<12 ; /* we shift to include 12 zero bits  */
        ptr = phys_to_virt(pt1) ; /*convert phys pt root to virtual*/
        uint64_t key ;
        for (int i =4 ; i>=0 ; i--) /*we dived the 45 bits into 5 parts so we loop 5 times*/
        {
            int shift = 9*i ;
            key = (vpn>>shift) & 0x1FF ; /*this extracts the 9 bits of level i*/
            if ((ptr[key] & 1) == 0 ) { /* check if ptr points to valid entry*/
                return NO_MAPPING;
            }
            if (i>0) /* we continue to the next level if i>0 , if i=0 we finished*/
            {
            pt1 = ptr[key]-1 ;
            ptr = phys_to_virt(pt1) ;  
            }
            
        }

        return ptr[key]>>12; /*return phys address shifted to exclude the first 12 bits */
}

