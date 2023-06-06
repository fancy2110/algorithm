fn main() {
    // mutable variable
    let mut x = 5;
    println!("The value of x is: {x} ");
    x = 6;
    println!("The value of x is: {x}");


    //showdow of variable
    {
        let  x = x + 1;
        println!("The value of x is: {x}");
    } 
    println!("The value of x is: {x}");

    let spaces = "     ";
    let spaces = spaces.len();
    println!("The spaces length is : {spaces}");

    let tup:(i32, f64, u8) = (500, 6.4, 1);
    let (tup0, tup1, tup2) = tup;
    println!("The tup is ({tup0}, {tup1}, {tup2})")
}
