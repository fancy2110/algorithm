use reference::add_one;
use rand::*;
fn main() {
    let sceret_number = rand::thread_rng().gen_range(1, 101); 

    let a = add_one(sceret_number);
    println!("origin:{}, result:{}", sceret_number, a);
}
