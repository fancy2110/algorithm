fn main() {
    let v = vec![1, 3, 4];
    println!("vector: {:?}", v);

    let second: &i32 = &v[2];
    println!("The second element is {}", second);

    match v.get(1) {
        Some(second) => println!("The Second element is {}", second),
        None => println!("There is no second element."),
    }
}
