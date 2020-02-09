use serde_derive::{Deserialize, Serialize};

pub enum MotorCommands {
    Forward,
    Reverse,
    Release,
}

impl MotorCommands {
    pub fn from_str(s: &str) -> Option<MotorCommands> {
        match s {
            "Forward" => Some(MotorCommands::Forward),
            "Reverse" => Some(MotorCommands::Reverse),
            "Release" => Some(MotorCommands::Release),
            _ => None,
        }
    }
    pub fn as_str(&self) -> &'static str {
        match &self {
            MotorCommands::Forward => "Forward",
            MotorCommands::Reverse => "Reverse",
            MotorCommands::Release => "Release",
        }
    }
}

#[derive(Serialize, Deserialize)]
pub struct Msg {
    pub wheel_speed: u8,
    pub left_side_dir: String,
    pub right_side_dir: String,
}
impl Msg {
    pub fn new() -> Msg {
        // Init to some default values
        Msg {
            wheel_speed: 0,
            left_side_dir: "Release".to_owned(),
            right_side_dir: "Forward".to_owned(),
        }
    }
}

pub enum Ds4Controls {
    // Should give stick axes around 20 of wiggle room
    LeftStickXAxis  { x : u8 }, // 0 is left, 127 is middle, 255 is right
    LeftStickYAxis  { y : u8 }, // 0 is up,   127 is middle, 255 is down
    RightStickXAxis { x : u8 }, // 0 is left, 127 is middle, 255 is right
    RightStickYAxis { y : u8 }, // 0 is up,   127 is middle, 255 is down
    LeftTrigger { val : u8 }, // 0 is none, 255 is fully pressed
    RightTrigger{ val : u8 }, // 0 is none, 255 is fully pressed
}

