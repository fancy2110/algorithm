use std::net::TcpListener;
use std::net::TcpStream;
use std::io::prelude::*;
use std::fs;
use std::time::Duration;
use std::thread;
use web_server::ThreadPool;

fn main() {
    let listener = TcpListener::bind("127.0.0.1:7878").unwrap();

    let pool = ThreadPool::new(10);

    for stream in listener.incoming().take(4) {
        let stream = stream.unwrap();  
        pool.execute(move || {
            handle_connection(stream);
            thread::sleep(Duration::from_secs(10));
        });
        println!("Connection established!");
    }
}

fn handle_connection(mut stream : TcpStream) {
    let mut buffer = [0; 1024];
    stream.read(&mut buffer).unwrap();
    println!("request content:{}", String::from_utf8_lossy(&buffer[..]));

    let index_get = b"GET / HTTP/1.1\r\n";
    if buffer.starts_with(index_get) {
        let contents = fs::read_to_string("hello.html").unwrap();
        let response = format!(
            "HTTP/1.1 200 OK\r\nContent-Length: {}\r\n\r\n{}",
            contents.len(),
            contents
        );
        stream.write(response.as_bytes()).unwrap();
        stream.flush().unwrap();
    } else {
        let status_line = "HTTP/1.1 404 NOT FOUND";
        let contents = fs::read_to_string("404.html").unwrap();
        let response = format!(
            "{}\r\nContent-Length: {}\r\n\r\n{}", 
            status_line,
            contents.len(),
            contents
        );
        
        stream.write(response.as_bytes()).unwrap();
        stream.flush().unwrap();
    }
} 
