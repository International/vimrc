// Copyright 2012 The Rust Project Developers. See the COPYRIGHT
// file at the top-level directory of this distribution and at
// http://rust-lang.org/COPYRIGHT.
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.

extern crate debug;

fn main() {
    let y: Box<int> = box 42;
    let mut x: Box<int>;
    loop {
        println!("{:?}", y);
        loop {
            loop {
                loop {
                    x = y; //~ ERROR use of moved value
                    x.clone();
                }
            }
        }
    }
}