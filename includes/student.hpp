#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <vector>

struct Student {
  std::string full_name;
  unsigned int uin = 0;
  double gpa = 0.0;
  std::vector<double> quiz_scores;
  bool is_cs_major;
};

#endif