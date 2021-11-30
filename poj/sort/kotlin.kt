import java.io.File;


fun main() {
    val dataFile = File("./data.txt");
    val data = mutableListOf(0, 100, 1, 2,8 , 3, 1000, 50);

    // dataFile.forEachLine() { it ->
        // data.add(it.toInt());
    // }

    val sortedData = insert_sort(data);
    sortedData.forEach {
        println(it);
    }

    println("quick_sort")
    quick_sort(data, 0, data.size - 1);
    data.forEach { 
        println(it)
     }
}

fun insert_sort(data :  List<Int>) :List<Int> {
    if (data.size <= 1) return data.toList();
    val newData = MutableList(0 , { -1; });
    newData.add(data.first());

    for (originIndex in 1 until data.size) {
        val nextItem = data[originIndex]
        var nextPosition = 0
        for (index in 0 until newData.size) {
            val currentItem = newData[index]
            nextPosition = index
            if (currentItem <= nextItem) {
                break;
            }
        }
        newData.add(nextPosition, nextItem)
    }

    return newData;
}

//0, 100, 1, 2,8 , 3, 1000, 50
fun quick_sort(data:MutableList<Int>, start:Int, end:Int) {
    if (end <= start) return 
    val mid = (end - start) / 2 + start

    var i = start; 
    var j = end;
    var flag = data[mid];

    while (i < j) {
        while(i < j && data[i] > flag) i ++ 
        while(i < j && data[j] < flag) j --

        if (i >= j) break

        val temp = data[i]
        data[i] = data[j]
        data[j] = temp
        i++
        j--
    } 

    val newMid = (i+j) / 2
    quick_sort(data, start , newMid);
    quick_sort(data, newMid + 1 , end);
}
