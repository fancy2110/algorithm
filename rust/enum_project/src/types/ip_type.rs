#[derive(Debug)]
enum IpAddrKind {
    V4,
    V6,
}

#[derive(Debug)]
struct IpAddr {
    kind: IpAddrKind,
    address: String,
}

#[derive(Debug)]
enum IpAddrV2 {
    V4(u8, u8, u8, u8),
    V6(String),
}

pub fn struct_style() {
    let home = IpAddr {
        kind: IpAddrKind::V4,
        address: String::from("127.0.0.1"),
    };

    let loopback = IpAddr {
        kind: IpAddrKind::V6,
        address: String::from("::1"),
    };

    println!("{:?}", home);
    println!("{:?}", loopback);
}

pub fn enum_style() {
    let home1 = IpAddrV2::V4(127, 0, 0, 1);
    let loopback1 = IpAddrV2::V6(String::from("::1"));
    println!("{:?}", home1);
    println!("{:?}", loopback1);
}


pub fn std_style() {
    let home = std::net::IpAddr::V4(std::net::Ipv4Addr::new(127, 0, 0, 1));
    let loopback = std::net::IpAddr::V6(std::net::Ipv6Addr::new(0, 0, 0, 0, 0, 0, 0, 1));
    println!("{:?}", home);
    println!("{:?}", loopback);
}