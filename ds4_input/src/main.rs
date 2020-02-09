extern crate evdev_rs as evdev;
extern crate nix;
extern crate serialport;

use serialport::prelude::*;
use evdev::*;
use evdev::enums::*;
use nix::errno::Errno;
use std::fs::File;

mod json_interface;

// Kind of difficult to match an event code to the EV_ABS enum, it could be my
// infamliarity with rust.
fn event_code_to_usage(ec: &EventCode) -> String {
    use evdev::enums::EV_ABS::*;
    match ec {
        // Turn each EV_ABS::* enum into an EventCode
        // Should give axes around 20 of wiggle room
        EventCode::EV_ABS(ABS_X)  => "left_stick_x_axis".to_string(),  // 0 is left, 127 is middle, 255 is right
        EventCode::EV_ABS(ABS_Y)  => "left_stick_y_axis".to_string(),  // 0 is up,   127 is middle, 255 is down
        EventCode::EV_ABS(ABS_RX) => "right_stick_x_axis".to_string(), // 0 is left, 127 is middle, 255 is right
        EventCode::EV_ABS(ABS_RY) => "right_stick_y_axis".to_string(), // 0 is up,   127 is middle, 255 is down
        EventCode::EV_ABS(ABS_Z)  => "left_trigger".to_string(),       // 0 is none, 255 is fully pressed
        EventCode::EV_ABS(ABS_RZ) => "right_trigger".to_string(),      // 0 is none, 255 is fully pressed
                                _ => "unknown".to_string(),
    }
}

fn update_msg_struct(ev: &InputEvent, mut lastMsg: &json_interface::Msg ) {
    match ev.event_code {
        // Update left/right dir if beyond bounds
        EventCode::EV_ABS(ABS_X) => {
            let difference = ev.value - 127;
            if abs(difference) > 20
                // TODO Translate the value to a direction

        },
        // Set reverse, variable speed
        EventCode::EV_ABS(ABS_Z) =>
        // Set forward, variable speed
        EventCode::EV_ABS(ABS_RZ) => 
        _ => return, // Do nothing
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

    let mut msg = json_interface::Msg::new();

    let port_path = "/dev/ttyACM0";
    let mut settings: SerialPortSettings = Default::default();
    settings.baud_rate = 9600;

    let port = serialport::open_with_settings(&port_path, &settings).unwrap();

    // Main event loop
    let mut a: Result<(ReadStatus, InputEvent), Errno>;
    loop {
        a = dev.next_event(evdev::ReadFlag::NORMAL | evdev::ReadFlag::BLOCKING);
        if a.is_ok() {
            let result = a.ok().unwrap();
            match result.0 {
                ReadStatus::Sync => continue,
                ReadStatus::Success => { 
                    update_msg_struct(&result.1, &msg);
                    let json_string = serde_json::to_string(&msg).unwrap();
                    port.write(&json_string.as_bytes());
                }
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