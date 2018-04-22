#include <pbc.h>
#include <pbc_test.h>
#define FILETYPES 3 // In this demo code, we assume here that there are totally 3 types of files.

typedef struct Secret_key{
  element_t a1, a2, a3;
} secret_key;
typedef struct Public_key{
  element_t part1; 
  element_t part2;
  element_t part3[5]; // index = 3 * 2 - 1 
} public_key;


int main(int argc, char **argv) {

  pairing_t pairing;
  pbc_demo_pairing_init(pairing, argc, argv);
  if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");
 
  double beginTime = 0, endTime = 0;
  
  element_t g, d, u, v, w; // g, d ,u, v, w are generators in G1 group
  element_t temp;
  element_t number[100]; 
  secret_key AliceSecretKey;
  secret_key BobSecretKey;
  public_key AlicePublicKey;
  public_key BobPublicKey;


  beginTime = pbc_get_time();  
  // Setup & init
  element_init_G1(g, pairing);
  element_init_G1(d, pairing);
  element_init_G1(u, pairing);
  element_init_G1(v, pairing);
  element_init_G1(w, pairing);
  element_init_Zr(temp, pairing);
  for(int i = 0; i < 100; i++) element_init_Zr(number[i], pairing);
  for(int i = 0; i < 100; i++) element_set_si(number[i], i);
//  for(int i = 0; i < 100; i++) element_printf("system parameter  = %B\n", number[i]);
  element_init_G1(AlicePublicKey.part1, pairing);
  element_init_G1(AlicePublicKey.part2, pairing);
  for(int i = 0; i < 5; i++) element_init_G1(AlicePublicKey.part3[i], pairing);
  element_init_G1(BobPublicKey.part1, pairing);
  element_init_G1(BobPublicKey.part2, pairing);
  for(int i = 0; i < 5; i++) element_init_G1(BobPublicKey.part3[i], pairing);
  element_init_Zr(AliceSecretKey.a1, pairing);  
  element_init_Zr(AliceSecretKey.a2, pairing);  
  element_init_Zr(AliceSecretKey.a3, pairing);  
  element_init_Zr(BobSecretKey.a1, pairing);  
  element_init_Zr(BobSecretKey.a2, pairing);  
  element_init_Zr(BobSecretKey.a3, pairing);
  element_random(g);
  element_random(d);
  element_random(u);
  element_random(v);
  element_random(w);
  element_random(AliceSecretKey.a1);
  element_random(AliceSecretKey.a2);
  element_random(AliceSecretKey.a3);
  element_random(BobSecretKey.a1);
  element_random(BobSecretKey.a2);
  element_random(BobSecretKey.a3);
  // KeyGen
  element_pow_zn(AlicePublicKey.part1, g, AliceSecretKey.a1); 
  element_pow_zn(AlicePublicKey.part2, g, AliceSecretKey.a2);
/* testing, actually it should not contain n+1 th 
  for(int i = 0; i < 5; i++) {
    element_pow_zn(temp, AliceSecretKey.a3, number[i+1]);
    element_pow_zn(AlicePublicKey.part3[i], g, temp);
    element_printf("number[%d] = %B\n", i, number[i+1]);
    element_printf("temp = %B\n", temp);
    element_printf("AlicePublicKey.part3[%d] = %B\n", i, AlicePublicKey.part3[i]);
  }
*/
  element_printf("system parameter AliceSecretKey.a1 = %B\n", AliceSecretKey.a1);
  element_printf("system parameter AliceSecretKey.a2 = %B\n", AliceSecretKey.a2);
  element_printf("system parameter AliceSecretKey.a3 = %B\n", AliceSecretKey.a3);
  element_printf("system parameter BobSecretKey.a1 = %B\n", BobSecretKey.a1);
  element_printf("system parameter BobSecretKey.a2 = %B\n", BobSecretKey.a2);
  element_printf("system parameter BobSecretKey.a3 = %B\n", BobSecretKey.a3);
  element_printf("system parameter g = %B\n", g);
  element_printf("system parameter d = %B\n", d);
  element_printf("system parameter u = %B\n", u);
  element_printf("system parameter v = %B\n", v);
  element_printf("system parameter w = %B\n", w);
  
  endTime = pbc_get_time();  
  printf("All time = %fs\n", endTime - beginTime);
  return 0;
}
