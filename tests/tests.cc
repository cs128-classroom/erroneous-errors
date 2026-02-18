// clang-format off
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//  Written By : <Your Name>                  Environment : ubuntu:bionic               //
//  Date ......: <00/00/00>                      Compiler ...: clang-10                    //
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// clang-format on
/////////////////////////////////////////////////////////////////////////////////////////////
//                             Framework Set-up //
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include "catch.hpp"
/////////////////////////////////////////////////////////////////////////////////////////////
//                                 Includes //
/////////////////////////////////////////////////////////////////////////////////////////////
#include <stdexcept>

#include "solution.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////
//                             Helpers/Constants //
/////////////////////////////////////////////////////////////////////////////////////////////
// check member variables of student structs are equivalent
bool operator==(const Student& s1, const Student& s2) {
  return s1.full_name == s2.full_name && s1.gpa == s2.gpa &&
         s1.quiz_scores == s2.quiz_scores && s1.uin == s2.uin;
}
const std::vector<Student> kExampleStudents = {
    Student(
        "Kanye West", 667, 2.5, {12.80294417, 42.73189462, 78.41290655}, false),
    Student("Taylor Swift",
            1234,
            3.11,
            {45.10377218, 80.11345829, 14.23361058},
            true),
    Student("Kim Kardashian",
            99999,
            4.0,
            {43.91840657, 79.64201783, 13.50187294},
            true),
    Student("Tom Cruise",
            140923,
            0.2,
            {44.28761193, 81.00498371, 15.14820936},
            false),
    Student("Cristiano Ronaldo",
            7,
            0.7,
            {46.00913488, 77.90266114, 13.90376521},
            false),
    Student("Elon Musk",
            1234123,
            0.01,
            {99.27461539, 2.31877406, 96.74125803},
            true)};
/////////////////////////////////////////////////////////////////////////////////////////////
//                                Test Cases //
/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("ReadCSVFile Non-existant File", "[ReadCSVFile]") {
  std::string invalid_path = "./testing_files/imaginary_file.csv";
  REQUIRE_THROWS(ReadCSVFile(invalid_path));
}
TEST_CASE("ReadCSVFile Improper CSV File", "[ReadCSVFile]") {
  std::string improper_csv = "./testing_files/improper.csv";
  REQUIRE_THROWS(ReadCSVFile(improper_csv));
}
TEST_CASE("ReadCSVFile General Case", "[ReadCSVFile]") {
  std::string input_csv = "./testing_files/example.csv";
  std::vector<Student> output = ReadCSVFile(input_csv);
  REQUIRE(output == kExampleStudents);
}
TEST_CASE("FilterStudents", "[FilterStudents]") {
  // bruh idc about magic numbers can you shut up clang-tidy
  std::vector<Student> cs_majors = {
      kExampleStudents[1], kExampleStudents[2], kExampleStudents[5]};
  std::vector<Student> not_cs_majors = {
      kExampleStudents[0], kExampleStudents[3], kExampleStudents[4]};
  std::vector<Student> output_cs_major = FilterStudents(kExampleStudents, true);
  std::vector<Student> output_not_cs_major =
      FilterStudents(kExampleStudents, false);
  REQUIRE(cs_majors == output_cs_major);
  REQUIRE(not_cs_majors == output_not_cs_major);
}

TEST_CASE("GetAverage Empty Case", "[GetAverage]") {
  REQUIRE_THROWS(GetAverage({}, 1));
}
TEST_CASE("GetAverage Invalid Quiz Number", "[GetAverage]") {
  REQUIRE_THROWS(GetAverage(kExampleStudents, 12039102));
  REQUIRE_THROWS(GetAverage(kExampleStudents, 0));
}
bool NearEquivalent(const double& d1, const double& d2) {
  double e = 0.001;
  return d1 <= (d2 + e) && d1 >= (d2 - e);
}
TEST_CASE("GetAverage General Case", "[GetAverage]") {
  double q1 = 48.5660808533;
  double q2 = 60.6189649416;
  double q3 = 38.6569371116;
  REQUIRE(NearEquivalent(q1, GetAverage(kExampleStudents, 1)));
  REQUIRE(NearEquivalent(q2, GetAverage(kExampleStudents, 2)));
  REQUIRE(NearEquivalent(q3, GetAverage(kExampleStudents, 3)));
}
TEST_CASE("FindOutliers, Even Number of Students", "[FindOutliers]") {
  std::vector<double> q1 = FindOutliers(kExampleStudents, 1);
  std::vector<double> q2 = FindOutliers(kExampleStudents, 2);
  std::vector<double> q3 = FindOutliers(kExampleStudents, 3);
  std::vector<double> expected_1 = {12.80294417, 99.27461539};
  std::vector<double> expected_2 = {};
  std::vector<double> expected_3 = {};
  REQUIRE(q1 == expected_1);
  REQUIRE(q2 == expected_2);
  REQUIRE(q3 == expected_3);
}

TEST_CASE("FindOutliers Invalid Quiz Number", "[FindOutliers]") {
  REQUIRE_THROWS(FindOutliers(kExampleStudents, 12039102));
  REQUIRE_THROWS(FindOutliers(kExampleStudents, 0));
}

std::vector<Student> DataToStudent(const std::vector<double>& v) {
  std::vector<Student> stu;
  for (unsigned int i = 0; i < v.size(); i++) {
    Student s;
    s.full_name = "";
    s.gpa = 0.0;
    s.is_cs_major = true;
    s.quiz_scores = {v[i]};
    s.uin = 0;
    stu.push_back(s);
  }
  return stu;
}

TEST_CASE("FindOutliers, Odd Number of Students", "[FindOutliers]") {
  std::vector<double> data = {41.23819457,
                              42.01938462,
                              42.77190514,
                              43.10587233,
                              43.88912005,
                              44.32761198,
                              99.81344217};
  std::vector<Student> students = DataToStudent(data);
  std::vector<double> expected = {99.81344217};
  REQUIRE(expected == FindOutliers(students, 1));
}
TEST_CASE("FindOutliers, data lies exactly on bounds", "[FindOutliers]") {
  std::vector<double> data = {14.0, 20.0, 22.0, 23.0, 24.0, 24.0, 35.0};
  std::vector<Student> students = DataToStudent(data);
  std::vector<double> expected = {35.0};
  REQUIRE(expected == FindOutliers(students, 1));
}
TEST_CASE("Calculate Grade Does not Cascade Backwards", "[Calculate Grade]") {
  std::vector<double> quiz_scores_input = {14.2, 11.7, 80.5, 90.123, 100};
  double expected_output = 68.1346;
  REQUIRE(NearEquivalent(expected_output, CalculateGrade(quiz_scores_input)));
}
bool CompareFiles(const std::string& p1, const std::string& p2) {
  std::ifstream f1(p1);
  std::ifstream f2(p2);

  if (f1.fail() || f2.fail()) {
    return false;  // file problem
  }

  std::string f1_read;
  std::string f2_read;
  while (f1.good() || f2.good()) {
    f1 >> f1_read;
    f2 >> f2_read;
    if (f1_read != f2_read || (f1.good() && !f2.good()) ||
        (!f1.good() && f2.good()))
      return false;
  }
  return true;
}
TEST_CASE("WriteGradeStatistics ", "[WriteGradeStatistics]") {
  WriteGradeStatistics("./testing_files/actual_output.txt", kExampleStudents);
  REQUIRE(CompareFiles("./testing_files/actual_output.txt",
                       "./testing_files/expected_output.txt"));
}
TEST_CASE("WriteGradeReport", "[WriteGradeReport]") {
  for (const Student& s : kExampleStudents) {
    WriteGradeReport(
        "./testing_files/actual_students_output/" + s.full_name + ".txt", s);
    REQUIRE(CompareFiles(
        "./testing_files/actual_students_output/" + s.full_name + ".txt",
        "./testing_files/expected_students_output/" + s.full_name + ".txt"));
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////