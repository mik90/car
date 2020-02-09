extern crate serialport;

use serde_derive::{Deserialize, Serialize};
use serialport::prelude::*;
use std::{thread, time};

#[derive(Serialize, Deserialize)]
struct Msg {
    wheel_speed: u8,
    left_side_dir: String,
    right_side_dir: String,
}
/* Ex:
    let msg = Msg{
            wheel_speed : 100.to_owned(),
            left_side_dir : "Forward".to_owned(),
            right_side_dir : "Forward".to_owned()
    };
    let j_str = serde_json::to_string(&msg).unwrap();
*/
fn init_port(baud_rate: u32, port_path: &String) -> Box<SerialPort> {

    let mut settings: SerialPortSettings = Default::default();
    settings.baud_rate = baud_rate;

    match serialport::open_with_settings(&port_path, &settings){
        Ok(port) => port,
        Err(e) => {
            eprintln!("Could not open port at {}, {:?}", &port_path, e.description);
            ()
        },
    }
}
