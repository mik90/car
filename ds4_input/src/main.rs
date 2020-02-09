extern crate evdev_rs as evdev;
extern crate nix;

use evdev::*;
use evdev::enums::*;
use nix::errno::Errno;
use std::fs::File;

//mod json_interface;

// Kind of difficult to match an event code to the EV_ABS enum, it could be my
// infamliarity with rust.
fn event_code_to_usage(ec: &EventCode) -> String {
    use evdev::enums::EV_ABS::*;
    match ec {
        // Turn each EV_ABS::* enum into an EventCode
        EventCode::EV_ABS(ABS_X)  => "left_stick_x_axis".to_string(),   // 0 is left, 127 is middle, 255 is right
        EventCode::EV_ABS(ABS_Y)  => "left_stick_y_axis".to_string(),   // 0 is up,   127 is middle, 255 is down
        EventCode::EV_ABS(ABS_RX)  => "right_stick_x_axis".to_string(), // 0 is left, 127 is middle, 255 is right
        EventCode::EV_ABS(ABS_RY) => "right_stick_y_axis".to_string(),  // 0 is up,   127 is middle, 255 is down
        EventCode::EV_ABS(ABS_Z) => "left_trigger".to_string(),         // 0 is none, 255 is fully pressed
        EventCode::EV_ABS(ABS_RZ) => "right_trigger".to_string(),       // 0 is none, 255 is fully pressed
                                _ => "".to_string(),
    }
}

fn handle_event(ev: &InputEvent) {
    match ev.event_type {
        // Currently only care about ABS events
        EventType::EV_ABS => {
            println!("Event:: code:{}, value:{}, usage:{}",
                      ev.event_code,
                      ev.value, 
                      event_code_to_usage(&ev.event_code));
        } ,
        // Discard others
        _ => return,
    }
}

fn main()
{
    let mut args = std::env::args();
    if args.len() != 2 {
        println!("Usage: ds4_input /path/to/event/device");
        std::process::exit(1);
    }

    let dev_path = &args.nth(1).unwrap(); // Grab the device path
    println!("Device path: {:?}:", dev_path);

    let dev_file = File::open(dev_path).unwrap();
    let mut dev = evdev::Device::new().unwrap();
    dev.set_fd(dev_file).unwrap();
    println!("evdev Device created");


    // Main event loop
    let mut a: Result<(ReadStatus, InputEvent), Errno>;
    loop {
        a = dev.next_event(evdev::ReadFlag::NORMAL | evdev::ReadFlag::BLOCKING);
        if a.is_ok() {
            let result = a.ok().unwrap();
            match result.0 {
                ReadStatus::Sync => continue,
                //ReadStatus::Success => handle_event(&result.1),
                ReadStatus::Success => print_triggers(&result.1),
            }
        }
        else {
            match a.err().unwrap() {
                Errno::EAGAIN => continue,
                err => { println!("{}", err); break;}
            }
        }
    }
}