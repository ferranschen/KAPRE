#include <pbc.h>
#include <pbc_test.h>
#define FILETYPES 3 // In this demo code, we assume here that there are totally 3 types of files.

typedef struct SECRET_KEY{
  element_t a1, a2, a3;
} secret_key;
typedef struct PUBLIC_KEY{
  element_t part1; 
  element_t part2;
  element_t part3[5]; // index = 3 * 2 - 1 
} public_key;
typedef struct SECOND_LEVEL_CIPHERTEXT{
  element_t c0, c1, c2, c3, c4, c5 , c6, c7, c8, c9; // c0 presents k in paper. 
} second_level_ciphertext;
typedef struct SECOND_LEVEL_PARAMETER{
  element_t t, k, r, n;
} second_level_parameter;
typedef struct FIRST_LEVEL_CIPHERTEXT{
  element_t c0, c1, c2, c3, c4, c5, c6;
} first_level_ciphertext;
typedef struct FIRST_LEVEL_PARAMETER{
  element_t k, r, n;
} first_level_parameter;

int main(int argc, char **argv) {

  pairing_t pairing;
  pbc_demo_pairing_init(pairing, argc, argv);
  if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");
 
  double beginTime = 0, endTime = 0;
  
  element_t g, d, u, v, w; // g, d ,u, v, w are generators in G1 group
  element_t tempZR;
  element_t tempG1;
  element_t tempGT;
  element_t number[100]; 
  secret_key AliceSecretKey;
  secret_key BobSecretKey;
  public_key AlicePublicKey;
  public_key BobPublicKey;
  second_level_ciphertext AliceSecondLevelCiphertext;
  first_level_ciphertext AliceFirstLevelCiphertext;
  second_level_parameter AliceSecondLevelParameter;
  first_level_parameter AliceFirstLevelParameter;

  beginTime = pbc_get_time();  
  // Setup & init
  element_init_G1(g, pairing);
  element_init_G1(d, pairing);
  element_init_G1(u, pairing);
  element_init_G1(v, pairing);
  element_init_G1(w, pairing);
  element_init_G1(tempG1, pairing);
  element_init_GT(tempGT, pairing);
  element_init_Zr(tempZR, pairing);


  for(int i = 0; i < 100; i++) element_init_Zr(number[i], pairing);
  for(int i = 0; i < 100; i++) element_set_si(number[i], i); // set nature number 0 ~ 100
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
  element_init_Zr(AliceSecondLevelCiphertext.c0, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c1, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c2, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c3, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c4, pairing);
  element_init_Zr(AliceSecondLevelCiphertext.c5, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c6, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c7, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c8, pairing);
  element_init_Zr(AliceSecondLevelCiphertext.c9, pairing);

  element_init_Zr(AliceFirstLevelCiphertext.c0, pairing);
  element_init_G1(AliceFirstLevelCiphertext.c1, pairing);
  element_init_GT(AliceFirstLevelCiphertext.c2, pairing);
  element_init_Zr(AliceFirstLevelCiphertext.c3, pairing);
  element_init_G1(AliceFirstLevelCiphertext.c4, pairing);
  element_init_G1(AliceFirstLevelCiphertext.c5, pairing);
  element_init_Zr(AliceFirstLevelCiphertext.c6, pairing);

  element_init_Zr(AliceSecondLevelParameter.t, pairing);
  element_init_Zr(AliceSecondLevelParameter.k, pairing);
  element_init_Zr(AliceSecondLevelParameter.r, pairing);
  element_init_Zr(AliceSecondLevelParameter.n, pairing);
  element_init_Zr(AliceFirstLevelParameter.k, pairing);
  element_init_Zr(AliceFirstLevelParameter.r, pairing);
  element_init_Zr(AliceFirstLevelParameter.n, pairing);

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
  // Key Generation
  element_pow_zn(AlicePublicKey.part1, g, AliceSecretKey.a1); 
  element_pow_zn(AlicePublicKey.part2, g, AliceSecretKey.a2);
  element_pow_zn(BobPublicKey.part1, g, BobSecretKey.a1);
  element_pow_zn(BobPublicKey.part2, g, BobSecretKey.a2);
  for(int i = 0; i < FILETYPES; i++) {

    element_pow_zn(tempZR, AliceSecretKey.a3, number[i+1]);
    element_pow_zn(AlicePublicKey.part3[i], g, tempZR);
    element_pow_zn(tempZR, BobSecretKey.a3, number[i+1]);
    element_pow_zn(BobPublicKey.part3[i], g, tempZR);
    
    if(i < FILETYPES - 1) {
      element_pow_zn(tempZR, AliceSecretKey.a3, number[FILETYPES + i + 1]);
      element_pow_zn(AlicePublicKey.part3[FILETYPES + i], g, tempZR);
      element_pow_zn(tempZR, BobSecretKey.a3, number[FILETYPES + i + 1]);
      element_pow_zn(BobPublicKey.part3[FILETYPES + i], g, tempZR);
    }

  }

  element_printf("AliceSecretKey.a1 = %B\n", AliceSecretKey.a1);
  element_printf("AliceSecretKey.a2 = %B\n", AliceSecretKey.a2);
  element_printf("AliceSecretKey.a3 = %B\n", AliceSecretKey.a3);
  for(int i = 0; i < FILETYPES*2 - 1;i++) {
    element_printf("AlicePublicKey.part3[%d] = %B\n", i, AlicePublicKey.part3[i]);
  }
  element_printf("BobSecretKey.a1 = %B\n", BobSecretKey.a1);
  element_printf("BobSecretKey.a2 = %B\n", BobSecretKey.a2);
  element_printf("BobSecretKey.a3 = %B\n", BobSecretKey.a3);
  for(int i = 0; i < FILETYPES*2 - 1;i++) {
    element_printf("BobPublicKey.part3[%d] = %B\n", i, BobPublicKey.part3[i]);
  }
  element_printf("system parameter g = %B\n", g);
  element_printf("system parameter d = %B\n", d);
  element_printf("system parameter u = %B\n", u);
  element_printf("system parameter v = %B\n", v);
  element_printf("system parameter w = %B\n", w);
  // ReKey Generation
  element_t r1, r2;  
  element_init_G1(r1, pairing);
  element_init_G1(r2, pairing);
  element_invert( tempZR, AliceSecretKey.a1);
  element_pow_zn(r1, BobPublicKey.part1, tempZR); // r1 = BobPublicKey.part1^(1/AliceSecretKey.a1)
//  element_printf("BobPublicKey.part1 = %B\n", BobPublicKey.part1); 
//  element_pow_zn(r1, r1, AliceSecretKey.a1); // r1 = BobPublicKey.part1^(1/AliceSecretKey.a1)
//  element_printf("r1 = %B\n", r1);
  element_mul(r2, AlicePublicKey.part3[2], AlicePublicKey.part3[1]); // We assume ALice share file set S = {1,2}
  element_pow_zn(r2, r2, AliceSecretKey.a2); //  
  // Enc2
  element_random(AliceSecondLevelParameter.t);
  element_random(AliceSecondLevelParameter.k);
  element_random(AliceSecondLevelParameter.r);
  element_random(AliceSecondLevelParameter.n);
  element_set(AliceSecondLevelCiphertext.c0, AliceSecondLevelParameter.k);
  element_pow_zn(AliceSecondLevelCiphertext.c1, d, AliceSecondLevelParameter.r);
  element_pow_zn(AliceSecondLevelCiphertext.c2, g, AliceSecretKey.a1);
  element_pow_zn(AliceSecondLevelCiphertext.c2, AliceSecondLevelCiphertext.c2, AliceSecondLevelParameter.r);
  element_pow_zn(AliceSecondLevelCiphertext.c3, g, AliceSecondLevelParameter.t);
  element_pow_zn(AliceSecondLevelCiphertext.c4, g, AliceSecretKey.a2);
  element_mul(AliceSecondLevelCiphertext.c4, AliceSecondLevelCiphertext.c4, AlicePublicKey.part3[0]);
  element_pow_zn(AliceSecondLevelCiphertext.c4, AliceSecondLevelCiphertext.c4, AliceSecondLevelParameter.t);
  // Enc1
  element_random(AliceFirstLevelParameter.k);
  element_random(AliceFirstLevelParameter.r);
  element_random(AliceFirstLevelParameter.n);
  element_set(AliceFirstLevelCiphertext.c0, AliceFirstLevelParameter.k);
  // Re-encrption
  
  // Dec1

  // Dec2
  endTime = pbc_get_time();  
  printf("All time = %fs\n", endTime - beginTime);
  return 0;
}
