fn main() {
    let str = String::from("Hello, world!");
    let first = first_word(&str);
    println!("{}", first);
}

fn first_word(s: &String) -> usize {
    let bytes = s.as_bytes();

    for (i, &item) in bytes.iter().enumerate() {
        if item == b' ' {
            return i;
        }
    }

    return bytes.len();
}
