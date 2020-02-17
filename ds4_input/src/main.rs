extern crate evdev_rs as evdev;
extern crate nix;
extern crate serialport;

use serialport::prelude::*;
use std::{thread, time};
use std::net::UdpSocket;

mod ds4_device;
use ds4_device::Ds4Device;
mod json_interface;
use json_interface::ds4_device_to_msg_json;

fn create_port() -> Box<dyn SerialPort> {
    let port_path = "/dev/ttyACM0";
    let mut settings: SerialPortSettings = Default::default();
    settings.baud_rate = 9600;
    settings.timeout = time::Duration::from_secs(2);

    match serialport::open_with_settings(&port_path, &settings) {
        Ok(v) => v,
        Err(e) => panic!(e),
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

    let mut ds4_dev = Ds4Device::new(dev_path).unwrap();

    let mut port = create_port();
    // Sleep while the arduino reboots
    println!("Sleeping for 3 seconds...");
    thread::sleep(time::Duration::from_secs(3));

    println!("Awake");
    let interval = time::Duration::from_millis(100);
    println!("Updating values every {:?} milliseconds", interval);
    let mut iteration = 0;
    let mut time_since_last_update = time::Instant::now();

    let socket = UdpSocket::bind("192.168.1.111:50001").expect("Couldn't bind UDP socket");
    let dest = "192.168.1.167:50001";

    // Main event loop
    loop {
        println!("Iteration:{}    ---------------------------------------------------------",
                 iteration);
        iteration += 1;
        ds4_dev.update_values();

        let now = time::Instant::now();

        // Only write to the serial port periodically
        if now - time_since_last_update > interval {
            time_since_last_update = now;

            let json = ds4_device_to_msg_json(&ds4_dev) + "\n";
            match socket.send_to(json.as_bytes(), dest) {
                Ok(v) => println!("Wrote {} bytes to {}", v, dest),
                Err(e) => eprintln!("Error:{}, couldn't write to {}", e, dest),
            }
            /*
            match port.write(json.as_bytes()) {
                Ok(v) => println!("Wrote {} bytes to {}", v, port.name().unwrap()),
                Err(e) => eprintln!("Error:{}, couldn't write to {}", e, port.name().unwrap()),
            }
            */

        }
    }
}