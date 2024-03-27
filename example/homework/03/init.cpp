#include <Kokkos_Core.hpp>
#include <cstdio>

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
  // Make View of length n > 1000 and add values
  int n = 2500;
  Kokkos::View<double*> a("a", n);
  Kokkos::parallel_for(n, KOKKOS_LAMBDA(const int i) {
    a(i) = i * i * 3.14;
  });
  Kokkos::fence();

  // create two additional views of same size and datatype
  Kokkos::View<double*> b("b", n);
  Kokkos::View<double*> c("c", n);

  // deep_copy
  Kokkos::Timer timer;
  Kokkos::deep_copy(b, a);
  Kokkos::fence();
  double time_deep_copy = timer.seconds();

  // user copy
  timer.reset();
  Kokkos::parallel_for(n, KOKKOS_LAMBDA(const int i) {
    c(i) = a(i);
  });
  Kokkos::fence();
  double time_user_copy = timer.seconds();
  
  // Output times 
  cout << "Time for deep_copy: " << time_deep_copy << endl;
  cout << "Time for user copy: " << time_user_copy << endl;
  }
  Kokkos::finalize();
}
