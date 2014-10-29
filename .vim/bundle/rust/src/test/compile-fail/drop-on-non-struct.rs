// Copyright 2012 The Rust Project Developers. See the COPYRIGHT
// file at the top-level directory of this distribution and at
// http://rust-lang.org/COPYRIGHT.
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.

impl Drop for int {
    //~^ ERROR the Drop trait may only be implemented on structures
    //~^^ ERROR cannot provide an extension implementation
    fn drop(&mut self) {
        println!("kaboom");
    }
}

fn main() {
}