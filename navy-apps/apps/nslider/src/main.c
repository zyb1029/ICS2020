#include <assert.h>
#include <cstddef>
int *a = NULL;
void render() {
  if (a != NULL) {
	  assert(0);
  }
  else {
	  int p;
	  a = &p;
  }
}

int main() {
  render();
  return 0;
}
