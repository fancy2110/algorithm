use std::thread;
use std::sync::mpsc;
use std::sync::Arc;
use std::sync::Mutex;

pub struct ThreadPool  {
    threads : Vec<Worker>,
    sender  : mpsc::Sender<Job>,
}

struct Worker {
    id  : usize,
    job : thread::JoinHandle<()>,
}

type Job = Box<dyn FnOnce() + Send + 'static>;

impl Worker {
    fn new(id:usize, receiver: Arc<Mutex<mpsc::Receiver<Job>>>) -> Worker {
        let job = thread::spawn(move|| loop {
            let job = receiver.lock().unwrap().recv().unwrap();

            println!("Worker {} got a job; executing.", id);

            job();
        });
        Worker {id, job}
    }
}


impl ThreadPool {

    pub fn new(size: usize) -> ThreadPool {
        assert!(size > 0);
        let mut threads = Vec::with_capacity(size);
        let (sender, receiver) = mpsc::channel();

        let receiver = Arc::new(Mutex::new(receiver));

        for id in 0..size {
            threads.push(Worker::new(id, Arc::clone(&receiver)));
        }

        ThreadPool {
            threads, sender
        }
    }

    pub fn execute<F>(&self,  f: F)
        where F:FnOnce() + Send + 'static 
    {
        let job = Box::new(f);
        self.sender.send(job).unwrap();
    }
}
