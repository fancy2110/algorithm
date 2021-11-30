import java.io.File

fun main() {

    val dataFileName = "./data.txt"
    val dataFile = File(dataFileName)

    for (i in 0..1000000) {
        dataFile.appendText((0..1000000).random().toString())
        dataFile.appendText("\n")
    }
}
