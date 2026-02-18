#include <iostream>

#include "solution.hpp"

int main() {
  std::vector<Student> students = ReadCSVFile("./testing_files/example.csv");
  WriteGradeStatistics("./testing_files/actual_output.txt", students);
  for (const Student& s : students) {
    WriteGradeReport(
        "./testing_files/actual_students_output/" + s.full_name + ".txt", s);
  }
  return 0;
}