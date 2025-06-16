use learn::rectangle::Rectangle;
#[test]
fn it_works() {
    let result = 2 + 2;
    assert_eq!(result, 4);
}

#[test]
fn not_works() {
    panic!("test failed")
}

#[test]
fn test_holde() {
    let big = Rectangle::square(12);
    let small = Rectangle::square(11);

    assert!(big.can_hold(&small))
}



