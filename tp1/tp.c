/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>
#include <string.h>


void userland() {
   asm volatile ("mov %eax, %cr0");
}

void print_gdt_content(gdt_reg_t gdtr_ptr) {
    seg_desc_t* gdt_ptr;
    gdt_ptr = (seg_desc_t*)(gdtr_ptr.addr);
    int i=0;
    while ((uint32_t)gdt_ptr < ((gdtr_ptr.addr) + gdtr_ptr.limit)) {
        uint32_t start = gdt_ptr->base_3<<24 | gdt_ptr->base_2<<16 | gdt_ptr->base_1;
        uint32_t end;
        if (gdt_ptr->g) {
            end = start + ( (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1) <<12) + 4095;
        } else {
            end = start + (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1);
        }
        debug("%d ", i);
        debug("[0x%x ", start);
        debug("- 0x%x] ", end);
        debug("seg_t: 0x%x ", gdt_ptr->type);
        debug("desc_t: %d ", gdt_ptr->s);
        debug("priv: %d ", gdt_ptr->dpl);
        debug("present: %d ", gdt_ptr->p);
        debug("avl: %d ", gdt_ptr->avl);
        debug("longmode: %d ", gdt_ptr->l);
        debug("default: %d ", gdt_ptr->d);
        debug("gran: %d ", gdt_ptr->g);
        debug("\n");
        gdt_ptr++;
        i++;
    }
}


void tp() {

    seg_desc_t dt [4];
    dt [0] = (seg_desc_t){ .limit_1 = 0, .base_1=0, .base_2=0, .type=0, .s=0, .dpl=0, .p=0, .limit_2=0, .avl=0, .l=0, .d=0, .g=0, .base_3=0};
    dt [1] = (seg_desc_t){ .limit_1 = 0x20, .base_1=0, .base_2=0, .type=SEG_DESC_CODE_XR, .s=0, .dpl=3, .p=1, .limit_2=0, .avl=0, .l=0, .d=1, .g=0, .base_3=0};
    dt [2] = (seg_desc_t){ .limit_1 = 0x20, .base_1=0, .base_2=0, .type=SEG_DESC_DATA_RW, .s=0, .dpl=3, .p=1, .limit_2=0, .avl=0, .l=0, .d=1, .g=0, .base_3=0};
    dt [3] = (seg_desc_t){ .limit_1 = 0x20, .base_1=0, .base_2=0x60, .type=SEG_DESC_DATA_RW, .s=0, .dpl=0, .p=1, .limit_2=0, .avl=0, .l=0, .d=1, .g=0, .base_3=0};

    gdt_reg_t new_gdt = {4 * sizeof(seg_desc_t),{.desc = &dt[0]}};
    set_gdtr(new_gdt);

	// TODO
    gdt_reg_t gdt ;
    get_gdtr(gdt);
    print_gdt_content(gdt);

    debug("valeur de cs :0x%x \n", get_cs());
    debug("valeur de ds :0x%x \n", get_ds());
    debug("valeur de ss :0x%x \n", get_ss());
    debug("valeur de es :0x%x \n", get_es());
    
    //set_ds(get_cs());
    debug("valeur de ds :0x%x \n", get_ds());

    //set_cs(get_ds());
    debug("valeur de cs :0x%x \n", get_cs());

    //set_es(0x18);
    char  src[64];
    char *dst = 0;

    memset(src, 0xff, 64);
    _memcpy8(dst, src, 32);

}
