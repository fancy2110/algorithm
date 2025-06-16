#[derive(Debug)]
struct Point<X, Y> {
    x: X,
    y: Y,
}

impl<X, Y> Point<X, Y> {
    fn merge<X1, Y1>(self, source: Point<X1, Y1>) -> Point<X, Y1> {
        Point {
            x: self.x,
            y: source.y,
        }
    }
}

pub fn test() {
    let p1 = Point { x: 'a', y: 1000 };
    let p2 = Point {
        x: "hello",
        y: false,
    };

    let p3 = p1.merge(p2);
    println!("{:?}", &p3)
}
