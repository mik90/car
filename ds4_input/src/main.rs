extern crate evdev_rs as evdev;
extern crate nix;
extern crate serialport;

use serialport::prelude::*;
use evdev::*;
use evdev::enums::*;
use nix::errno::Errno;
use std::fs::File;
use std::{thread, time};

mod json_interface;

fn x_axis_to_dirs(x: i32) -> Option<(String, String)> {
    // Significant enough to use
    if x > 147 {// Greater than the mid-point + 20 is right
        // Tank tread so make the left go forward and the right reverse
        // TODO This is awfully messy, fix it
        Some(("Forward".to_owned(), "Reverse".to_owned()))
    }
    else if x < 107 {
        Some(("Reverse".to_owned(), "Forward".to_owned()))
    }
    else {
        None
    }
}

fn update_msg_struct(ev: &InputEvent, last_msg: &mut json_interface::Msg ) {
    match &ev.event_code {
        // Update left/right dir if beyond bounds
       EventCode::EV_ABS(EV_ABS::ABS_X) => {
            if let Some(dir_pair) = x_axis_to_dirs(ev.value) {
                last_msg.left_side_dir = dir_pair.0;
                last_msg.right_side_dir = dir_pair.1;
            }
        },
        // Set reverse, variable speed
        EventCode::EV_ABS(EV_ABS::ABS_Z) => {
            last_msg.left_side_dir = "Reverse".to_owned();
            last_msg.right_side_dir = "Reverse".to_owned();
            last_msg.wheel_speed = ev.value as u8;
        }
        // Set forward, variable speed
        EventCode::EV_ABS(EV_ABS::ABS_RZ) => {
            last_msg.left_side_dir = "Forward".to_owned();
            last_msg.right_side_dir = "Forward".to_owned();
            last_msg.wheel_speed = ev.value as u8;
        }
        _ => return, // Do nothing
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

    let mut port = serialport::open_with_settings(&port_path, &settings).unwrap();

    // Sleep while the arduino reboots
    println!("Sleeping for 3 seconds...");
    thread::sleep(time::Duration::from_secs(3));
    println!("Awake");

    // Main event loop
    let mut a: Result<(ReadStatus, InputEvent), Errno>;
    loop {
        a = dev.next_event(evdev::ReadFlag::NORMAL | evdev::ReadFlag::BLOCKING);
        if a.is_ok() {
            let result = a.ok().unwrap();
            match result.0 {
                ReadStatus::Sync => continue,
                ReadStatus::Success => { 
                    update_msg_struct(&result.1, &mut msg);
                    let json_string = serde_json::to_string(&msg).unwrap();
                    port.write(&json_string.as_bytes())
                        .expect("Unable to write bytes");
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