// This will do the same thing as eventHandler.cpp except using
// the rust libevdev implementation

extern crate evdev_rs as evdev;
extern crate nix;

use evdev::*;
use evdev::enums::*;
use nix::errno::Errno;
use std::fs::File;

fn event_code_to_usage(ec: &EventCode) -> String
{
    use evdev::enums::EV_ABS::*;
    match ec
    {
        EventCode::EV_ABS(ABS_X)  => "left_stick_x_axis".to_string(),
        EventCode::EV_ABS(ABS_Y)  => "left_stick_y_axis".to_string(),
        EventCode::EV_ABS(ABS_Z)  => "right_stick_x_axis".to_string(),
        EventCode::EV_ABS(ABS_RZ) => "right_stick_y_axis".to_string(),
        EventCode::EV_ABS(ABS_RX) => "left_trigger".to_string(),
        EventCode::EV_ABS(ABS_RY) => "right_trigger".to_string(),
        _ => "".to_string(),
    }
}

fn handle_event(ev: &InputEvent)
{

    match ev.event_type
    {
        EventType::EV_ABS => 
        {
            println!("Event:: code:{}, value:{}, usage:{}",
                      ev.event_code,
                      ev.value, 
                      event_code_to_usage(&ev.event_code));
        } ,
        _ => return,
    }
}

fn main()
{
    let mut args = std::env::args();

    if args.len() != 2
    {
        println!("Usage: ps4Input /path/to/event/device");
        std::process::exit(1);
    }

    println!("DEBUG: args:{:?}", args);

    let dev_path = &args.nth(1).unwrap(); // Grab the device path
    println!("DEBUG: devPath: {:?}:", dev_path);

    let dev_file = File::open(dev_path).unwrap();
    let mut dev = evdev::Device::new().unwrap();
    dev.set_fd(dev_file).unwrap();
    println!("DEBUG: Device created");

    let mut a: Result<(ReadStatus, InputEvent), Errno>;
    loop
    {
        a = dev.next_event(evdev::ReadFlag::NORMAL | evdev::ReadFlag::BLOCKING);
        if a.is_ok()
        {
            let result = a.ok().unwrap();
            match result.0
            {
                ReadStatus::Sync => continue,
                ReadStatus::Success => handle_event(&result.1),

            }
        }
        else
        {
            match a.err().unwrap()
            {
                Errno::EAGAIN => continue,
                err => { println!("{}", err); break;}
            }
        }
    }
}