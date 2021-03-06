use serde_derive::{Deserialize, Serialize};
use crate::ds4_device::Ds4Device;


pub fn msg_is_duplicate(msg: &Msg, last_msg: &Msg) -> bool {
    // A new 
    if msg.left_side_dir != last_msg.left_side_dir
        ||  msg.right_side_dir != last_msg.right_side_dir {
        return false
    }

    let speed_fuzz = 20;
    use std::cmp::{max, min};
    let difference = max(msg.wheel_speed, last_msg.wheel_speed)
                        - min(msg.wheel_speed, last_msg.wheel_speed);
    difference < speed_fuzz
}

#[derive(Serialize, Deserialize)]
pub struct Msg {
    pub wheel_speed: u8,
    pub left_side_dir: MotorCommand,
    pub right_side_dir: MotorCommand,
}
impl Msg {
    pub fn new() -> Msg {
        // Init to some default values
        Msg {
            wheel_speed: 0,
            left_side_dir: MotorCommand::Release,
            right_side_dir: MotorCommand::Release,
        }
    }
}

#[derive(Serialize, Deserialize, PartialEq)]
pub enum MotorCommand {
    Forward,
    Reverse,
    Release,
}

impl MotorCommand {
    /*
    pub fn from_str(s: &str) -> MotorCommand {
        match s {
            "Forward" => MotorCommand::Forward,
            "Reverse" => MotorCommand::Reverse,
            "Release" => MotorCommand::Release,
            _         => MotorCommand::Release,
        }
    }
    pub fn as_str(&self) -> &'static str {
        match &self {
            MotorCommand::Forward => "Forward",
            MotorCommand::Reverse => "Reverse",
            MotorCommand::Release => "Release",
        }
    }
    */
}

fn left_stick_x_axis_to_dirs(x: u8, go_forward: bool) -> (MotorCommand, MotorCommand) {
    use MotorCommand::*;
    // Tank-tread style controls
    match x {
        0..=24    => (Reverse, Forward), // Left
        25..=107  => (Release, Forward), // Kinda left
        108..=147 => {
            // Not left or right, this is the fuzzy area in the middle.
            match go_forward {
                // Without instruction to turn from the x analog stick,
                // we have to default our direction from the triggers
                true  => (Forward, Forward), 
                false => (Reverse, Reverse), 
            }
        }
        148..=230 => (Forward, Release), // Kinda right
        231..=255 => (Forward, Reverse), // Right
    }
}

pub fn ds4_device_to_msg(dev: &Ds4Device) -> Msg  {
    // Can be negative, negativity implies reverse
    let speed_signed = dev.right_trigger as i16 - dev.left_trigger as i16;
    let go_forward;
    if speed_signed >= 0 {
        go_forward = true
    }
    else {
        go_forward = false
    }
    let absolute_difference = std::cmp::max(dev.left_trigger, dev.right_trigger)
                                 - std::cmp::min(dev.left_trigger, dev.right_trigger);
    let (left, right) = left_stick_x_axis_to_dirs(dev.left_stick_x_axis, go_forward);
    Msg {
        // Since you can press both triggers at the same time, the 
        // efforts on the triggers should cancel each other out
        // E.g. pressing 200 on left but 245 on right results in 45
        wheel_speed: absolute_difference,
        left_side_dir: left,
        right_side_dir: right,
    }
}

pub fn msg_to_json_string(msg: &Msg) -> String {
    match serde_json::to_string(msg) {
        Ok(v) => {
            println!("Ok: created json string:{}", v);
            v + "\n"
        },
        Err(e) => {
            eprintln!("Couldn't create json string:{}", e);
            "".to_owned()
        }
    }
}
