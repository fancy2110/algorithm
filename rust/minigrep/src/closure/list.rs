#[derive(PartialEq, Debug)]
pub struct Shoe {
    pub size: i32,
    pub style: String,
}

pub fn shoes_fit_my_size(shoes: Vec<Shoe>, shoe_size: i32) -> Vec<Shoe> {
    shoes.into_iter().filter(|s| s.size == shoe_size).collect()
}
