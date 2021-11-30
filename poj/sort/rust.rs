
fn main() {
    let mut data:[i32; 8] = [0, 100, 1, 2, 8, 3, 1000, 50];
    
    let new_data = insert_sort(&data);

    println!("new_data {:?}", new_data);

    quick_sort(&mut data, 0, 7);
    println!("data {:?}", data);
}

fn insert_sort(data: &[i32; 8]) -> [i32; 8] {
    let mut new_data: [i32; 8] = [-1; 8];

    new_data[0] = data[0];
    let mut new_data_size = 1;
    for index in 1..8 {
        let mut insert_position = 0;
        for new_index in 0..new_data_size {
            insert_position = new_index;
            if data[index] > new_data[new_index] {
                break;
            }
        } 

        new_data_size += 1;
        for item_index in ((insert_position+1)..new_data_size).rev() {
            new_data[item_index] = new_data[item_index - 1]; 
        }
        new_data[insert_position] = data[index];
    }

    return new_data;
}

fn quick_sort(data: &mut[i32; 8], start: usize, end:usize) {
    if end <=  start { return; }

    let mid = (end - start) / 2 + start;
    let mut i = start;
    let mut j = end;
    let flag = data[mid];

    while i < j {
        while i < j  &&  data[i] < flag {
            i += 1;
        }

        while i < j  && data[j] > flag  {
            j -= 1;
        }

        if i < j  {
            let tmp = data[i];
            data[i] = data[j];
            data[j] = tmp;
            i += 1;
            j -= 1;
        } 
    }

    let new_mid = (i + j) / 2;
    println!("start:{:?}, end:{:?}, i:{:?}, j:{:?},  data, {:?}", start, end, i, j ,data);
    quick_sort(data, start, new_mid); 
    quick_sort(data, new_mid + 1, end);
}