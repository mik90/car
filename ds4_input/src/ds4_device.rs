extern crate evdev_rs as evdev;

use evdev::enums::{EventCode, EV_ABS};
use nix::errno::Errno;
use std::fs::File;

pub struct Ds4Device {
    evdev_device : evdev::Device,
    pub left_stick_x_axis: u8,
    pub left_trigger: u8,
    pub right_trigger: u8,
}

impl Ds4Device {
    pub fn update_values(&mut self) {

        let event = self.evdev_device.next_event(evdev::ReadFlag::NORMAL | evdev::ReadFlag::BLOCKING);
        if event.is_err() {
            match event.err().unwrap() {
                Errno::EAGAIN => eprintln!("Could not get next_event, error is EAGAIN"),
                err => eprintln!("{}", err),
            }
            return
        }
        // Don't update the value if there's an error
        match self.evdev_device.event_value(&EventCode::EV_ABS(EV_ABS::ABS_X)){
            Some(v) => self.left_stick_x_axis = v as u8,
            None => eprintln!("Could not get event_value ABS_X"),
        }
        match self.evdev_device.event_value(&EventCode::EV_ABS(EV_ABS::ABS_Z)) {
            Some(v) => self.left_trigger = v as u8,
            None => eprintln!("Could not get event_value ABS_Z"),
        }
        match self.evdev_device.event_value(&EventCode::EV_ABS(EV_ABS::ABS_RZ)) {
            Some(v) => self.right_trigger = v as u8,
            None => eprintln!("Could not get event_value ABS_RZ"),
        }
        print!("update_values(): ");
        print!("left_stick_x_axis:{}, ",self.left_stick_x_axis);
        print!("left_trigger:{}, ",self.left_trigger);
        println!("right_trigger:{}",self.right_trigger);
    }
    pub fn new(event_device_path : &String) -> Option<Ds4Device> {
        let mut dev = evdev::Device::new().unwrap();
        let dev_file;
        match File::open(event_device_path) {
            Ok(v) => dev_file = v,
            Err(e) => {
                eprintln!("Could not open device file:{} due to:{}",
                          event_device_path, e);
                return None
            }
        }
        dev.set_fd(dev_file).unwrap();
        Some(Ds4Device {
            evdev_device: dev,
            left_stick_x_axis: 127,
            left_trigger: 0,
            right_trigger: 0,
        })
    }
}

/*
pub enum Ds4Controls {
    // Should give stick axes around 20 of wiggle room
    LeftStickXAxis  { x : u8 }, // 0 is left, 127 is middle, 255 is right
    LeftStickYAxis  { y : u8 }, // 0 is up,   127 is middle, 255 is down
    RightStickXAxis { x : u8 }, // 0 is left, 127 is middle, 255 is right
    RightStickYAxis { y : u8 }, // 0 is up,   127 is middle, 255 is down
    LeftTrigger { val : u8 }, // 0 is none, 255 is fully pressed
    RightTrigger{ val : u8 }, // 0 is none, 255 is fully pressed
}
*/