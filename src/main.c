#include "z3d.h"
#include "advance.h"

z3d_xyz_t pos;
z3d_rot_t rot;
uint32_t entrance;
uint8_t  entrance_set = 0;
//uint32_t age;
//uint8_t  age_set = 0;
uint8_t item_set_mode_state = 0; //1 for initialized, 2 for item slot set
uint8_t item_slot_chosen = 0;
//uint8_t item_chosen = 0;

advance_ctx_t advance_ctx = {};
advance_input_t inputs = {};
uint8_t advance_init;

void toggle_advance();

void scan_inputs() {
    inputs.cur.val = real_hid.pad.pads[real_hid.pad.index].curr.val;
    inputs.pressed.val = (inputs.cur.val) & (~inputs.old.val);
    inputs.up.val = (~inputs.cur.val) & (inputs.old.val);
    inputs.old.val = inputs.cur.val;
}

void store_pos() {
    pos = link.pos_1;
    rot = link.rot_1;
}

void restore_pos() {
    link.pos_1 = pos;
    link.pos_2 = pos;
    link.rot_1 = rot;
    link.rot_2 = rot;
    link.rot_3 = rot;
}

void advance_main() {

    scan_inputs();

    // /* Inventory edit code begins here
    // ---------------------------*/
    // //Hold R and press d-down to initialize
    // //Use d-up to increase by one
    // //Use d-right to increase by ten
    // //Use d-left to write null
    if (inputs.pressed.d_down && inputs.cur.r) {
        switch(item_set_mode_state) {
            case 0 : //not init
                item_set_mode_state = 1;
                z3d_file.rupee_count = 0;
                break;
            case 1 : //init, slot not yet set
                item_set_mode_state = 2;
                item_slot_chosen = z3d_file.rupee_count;
                z3d_file.rupee_count = 0;
                break;
            case 2:  //item slot set
                item_set_mode_state = 0;
                z3d_file.item_slot_item[item_slot_chosen] = z3d_file.rupee_count;
                break;
        }
    }
    if (inputs.pressed.d_up && item_set_mode_state) {
        z3d_file.rupee_count++;
    }
    if (inputs.pressed.d_right && item_set_mode_state) {
        z3d_file.rupee_count += 10;
    }
    //in this case, need to set visual item slots to slot ff as well
    if (inputs.pressed.d_left && (item_set_mode_state == 2)) {
        item_set_mode_state = 0;
        z3d_file.item_slot_item[item_slot_chosen] = 0xff;

        for (int i = 0; i < 0x30; ++i) {
            if (*(z3d_file.inventory_grid_data_child + i) == item_slot_chosen)
                *(z3d_file.inventory_grid_data_child + i) = 0xff;
        }

        return;
    }
    //panic button
    if (inputs.cur.l && inputs.cur.r) {
        item_set_mode_state = 0;
    }
    //if in item set mode, no other codes activate
    if (item_set_mode_state) return;

    /* (Re)store Position code
    ---------------------------*/
    //Stores Link's position
    if(inputs.pressed.d_left && inputs.cur.l) {
        store_pos();
    }

    //Restores Link's position
    if(inputs.pressed.d_right) {
        restore_pos();
    }

    /* Actor collision code */
    //Toggles actor collision viewer
    if((inputs.cur.sel && inputs.pressed.strt) || (inputs.pressed.sel && inputs.cur.strt)) {
        actor_collision = 1 - actor_collision;
    }

    /* ISG code
     ---------------------------*/
    //Gives ISG
    //Disabled for now because pointless and takes up space/buttons
    //if(inputs.pressed.d_down) {
        //link.isg = 0x01;
    //}
    //
    // /* Entrance set code. Early on in development
    // ---------------------------*/
    // //Right now sets entrance based on sticks/nuts/bombs amount
    if(inputs.pressed.d_right && inputs.cur.r) {
        entrance = (z3d_file.item_slot_amount[0] * 0x0100);
        entrance += (z3d_file.item_slot_amount[1] * 0x0010);
        entrance += (z3d_file.item_slot_amount[2] * 0x0001);
    
        //For the future when we have fadeout
        //memcpy(&z3d_file, z3d_file.item_slot_amount, 2);
        //turned off for now
        //entrance_set = 1;
    }

    /* Age swap code
    still does not work, idk
    ---------------------------*/
    // if(inputs.pressed.d_right && inputs.cur.l && inputs.cur.r){
    //     globalContext.pending_age = 1 - z3d_file.link_age;
    // }

    /*Frame Advance begins here
    Stolen from n3rdswithgame <3
    ---------------------------*/
    if(advance_init == 0) {
        advance_init = 1;
    }

    //up to enable, left to advance
    toggle_advance();
    scan_inputs();


    if(advance_ctx.advance_state == STEP) {
        if(inputs.cur.d_down) {
            advance_ctx.advance_state = LATCHED;
        } else {
            advance_ctx.advance_state = PAUSED;
        }
    }


    while(advance_ctx.advance_state == PAUSED || advance_ctx.advance_state == LATCHED) {
        scan_inputs();
        toggle_advance();
        if(advance_ctx.advance_state == LATCHED && !inputs.cur.d_down) {
            advance_ctx.advance_state = PAUSED;
        }
        if(advance_ctx.advance_state == PAUSED && inputs.cur.d_down) {
            advance_ctx.advance_state = STEP;
        }
        svcSleepThread(16e6);
    }

}

void area_load_main() {

    if (entrance_set) {
        z3d_file.entrance_index = entrance;
    }
    entrance_set = 0;
}

//TODO: change d_down_latched to d_up_latched or whatever final button code is
void toggle_advance() {
    //scan_inputs();
    if(inputs.pressed.d_up && advance_ctx.advance_state == NORMAL && !advance_ctx.d_down_latched){
        advance_ctx.advance_state = PAUSED;
        advance_ctx.d_down_latched = 1;
    } else if(inputs.pressed.d_up && advance_ctx.advance_state != NORMAL && !advance_ctx.d_down_latched) {
        advance_ctx.advance_state = NORMAL;
        advance_ctx.d_down_latched = 1;
    } else if (advance_ctx.advance_state == NORMAL && inputs.pressed.d_down){
        advance_ctx.advance_state = LATCHED;
    } else if(!inputs.pressed.d_up) {
        advance_ctx.d_down_latched = 0;
    }
}
