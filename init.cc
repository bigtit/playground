// how special members in a class are initialized

class A {
  static const int x;
  static int& y;
  static const int& a;
  const int& b;
  const int c;
  int& d;
public:
  A(int& x): /*a(1), wrong since a is static firstly*/b(1), c(1), d(x) {}
};

int t = 1;
const int& A::a = 1; // correct
const int A::x = 1;
int& A::y = t;

int main() {
  int a = 1;
  A t(a);
}
