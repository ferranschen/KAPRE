#include <pbc.h>
#include <pbc_test.h>
#include <string.h>
#define FILETYPES 3 // In this demo code, we assume here that there are totally 3 types of files.

typedef struct SECRET_KEY{
  element_t a1, a2, a3;
} secret_key;
typedef struct PUBLIC_KEY{
  element_t part1; 
  element_t part2;
  element_t part3[5]; // index = 3 * 2 - 1 
} public_key;
typedef struct CIPHER{
  element_t G1_type;
  unsigned long long_type;
} cipher;
typedef struct SECOND_LEVEL_CIPHERTEXT{
  element_t c0, c1, c2, c3, c4, c6, c7, c8, c9; // c0 presents k in paper. 
  cipher c5;
} second_level_ciphertext;
typedef struct SECOND_LEVEL_PARAMETER{
  element_t t, k, r, n;
} second_level_parameter;
typedef struct FIRST_LEVEL_CIPHERTEXT{
  element_t c0, c1, c2, c4, c5, c6;
  cipher c3;
} first_level_ciphertext;
typedef struct FIRST_LEVEL_PARAMETER{
  element_t k, r, n;
} first_level_parameter;


unsigned long H(element_t first, element_t second, pairing_t pairing);
unsigned long H1(element_t first, element_t second, pairing_t pairing);
unsigned long hash(unsigned char *str);




int main(int argc, char **argv) {

  pairing_t pairing;
  pbc_demo_pairing_init(pairing, argc, argv);
  if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");
  unsigned long m = 1024; //you can specify any number, here we use 1024 as an example
  unsigned long temp = 0;
  double beginTime = 0, endTime = 0;
  element_t g, d, u, v, w; // g, d ,u, v, w are generators in G1 group
  element_t tempZR;
  element_t tempG1;
  element_t tempG1_2;
  element_t tempGT;
  element_t tempGT_2;
  element_t number[100]; 
  element_t Z;
  secret_key AliceSecretKey;
  secret_key BobSecretKey;
  public_key AlicePublicKey;
  public_key BobPublicKey;
  second_level_ciphertext AliceSecondLevelCiphertext;
  first_level_ciphertext AliceFirstLevelCiphertext;
  second_level_parameter AliceSecondLevelParameter;
  first_level_parameter AliceFirstLevelParameter;
  first_level_ciphertext BobFirstLevelCiphertext;
  first_level_parameter BobFirstLevelParameter;

  beginTime = pbc_get_time();  

// -------------------------------
  
  // Setup & init
  element_init_G1(g, pairing);
  element_init_G1(d, pairing);
  element_init_G1(u, pairing);
  element_init_G1(v, pairing);
  element_init_G1(w, pairing);
  element_init_G1(tempG1, pairing);
  element_init_G1(tempG1_2, pairing);
  element_init_GT(tempGT, pairing);
  element_init_GT(tempGT_2, pairing);
  element_init_Zr(tempZR, pairing);
  element_init_GT(Z, pairing);

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
  element_init_G1(AliceSecondLevelCiphertext.c5.G1_type, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c6, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c7, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c8, pairing);
  element_init_G1(AliceSecondLevelCiphertext.c9, pairing);

  element_init_Zr(AliceFirstLevelCiphertext.c0, pairing);
  element_init_G1(AliceFirstLevelCiphertext.c1, pairing);
  element_init_GT(AliceFirstLevelCiphertext.c2, pairing);
  element_init_G1(AliceFirstLevelCiphertext.c3.G1_type, pairing);
  element_init_G1(AliceFirstLevelCiphertext.c4, pairing);
  element_init_G1(AliceFirstLevelCiphertext.c5, pairing);
  element_init_G1(AliceFirstLevelCiphertext.c6, pairing);

  
  element_init_Zr(BobFirstLevelCiphertext.c0, pairing);
  element_init_G1(BobFirstLevelCiphertext.c1, pairing);
  element_init_GT(BobFirstLevelCiphertext.c2, pairing);
  element_init_G1(BobFirstLevelCiphertext.c3.G1_type, pairing);
  element_init_G1(BobFirstLevelCiphertext.c4, pairing);
  element_init_G1(BobFirstLevelCiphertext.c5, pairing);
  element_init_G1(BobFirstLevelCiphertext.c6, pairing);


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
  element_pairing(Z, g, g);
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

  // ReKey Generation
  element_init_G1(tempG1, pairing);
  element_init_G1(tempG1_2, pairing);
  element_init_GT(tempGT, pairing);
  element_init_GT(tempGT_2, pairing);
  element_init_Zr(tempZR, pairing);

  element_t r1, r2;  
  element_init_G1(r1, pairing);
  element_init_G1(r2, pairing);
  element_invert(tempZR, AliceSecretKey.a1);
  element_pow_zn(r1, BobPublicKey.part1, tempZR); // r1 = BobPublicKey.part1^(1/AliceSecretKey.a1)
  element_mul(r2, AlicePublicKey.part3[2], AlicePublicKey.part3[1]); // We assume ALice share file set S = {1,2}
  element_pow_zn(r2, r2, AliceSecretKey.a2); //  

  // Enc2
  element_init_G1(tempG1, pairing);
  element_init_G1(tempG1_2, pairing);
  element_init_GT(tempGT, pairing);
  element_init_GT(tempGT_2, pairing);
  element_init_Zr(tempZR, pairing);

  element_t K;
  element_init_GT(K, pairing);
  element_random(AliceSecondLevelParameter.t);
  element_random(AliceSecondLevelParameter.k);
  element_random(AliceSecondLevelParameter.r);
  element_random(AliceSecondLevelParameter.n);

  element_set(AliceSecondLevelCiphertext.c0, AliceSecondLevelParameter.k);
  element_pow_zn(AliceSecondLevelCiphertext.c1, d, AliceSecondLevelParameter.r);
  element_set(AliceSecondLevelCiphertext.c2, AlicePublicKey.part1);
  element_pow_zn(AliceSecondLevelCiphertext.c2, AliceSecondLevelCiphertext.c2, AliceSecondLevelParameter.r);
  element_pow_zn(AliceSecondLevelCiphertext.c3, g, AliceSecondLevelParameter.t);
  element_mul(AliceSecondLevelCiphertext.c4, AlicePublicKey.part2, AlicePublicKey.part3[0]);
  element_pow_zn(AliceSecondLevelCiphertext.c4, AliceSecondLevelCiphertext.c4, AliceSecondLevelParameter.t);

  element_pow_zn(K, Z, AliceSecondLevelParameter.r);
  element_pairing(tempGT, AlicePublicKey.part3[0], AlicePublicKey.part3[FILETYPES - 1]);
  element_pow_zn(tempGT, tempGT, AliceSecondLevelParameter.t);
  
  temp = m ^ H1(K, AliceSecondLevelCiphertext.c1, pairing) ^ H1(tempGT, AliceSecondLevelCiphertext.c1, pairing);
  element_set_si(tempZR, temp);
  AliceSecondLevelCiphertext.c5.long_type = temp;
  element_pow_zn(AliceSecondLevelCiphertext.c5.G1_type, g, tempZR);
 
  element_t h, h1; // h and h prime
  element_init_Zr(h, pairing);
  element_init_Zr(h1, pairing);
  element_set_si(h,H(AliceSecondLevelCiphertext.c1, AliceSecondLevelCiphertext.c5.G1_type, pairing));
  temp = H1(K, AliceSecondLevelCiphertext.c1, pairing);
  temp = temp ^ m;
  element_set_si(tempZR, temp);
  element_pow_zn(tempG1, g, tempZR);
  element_set_si(h1, H(AliceSecondLevelCiphertext.c1, tempG1, pairing));
  element_set(tempG1, u);
  element_pow_zn(tempG1, tempG1, h);
  element_set(tempG1_2, v);
  element_pow_zn(tempG1_2, tempG1_2, AliceSecondLevelParameter.k);
  element_mul(AliceSecondLevelCiphertext.c6, tempG1, tempG1_2);
  element_mul(AliceSecondLevelCiphertext.c6, AliceSecondLevelCiphertext.c6, w);
  element_pow_zn(AliceSecondLevelCiphertext.c6, AliceSecondLevelCiphertext.c6, AliceSecondLevelParameter.r);

  element_set(tempG1, u);
  element_pow_zn(tempG1, tempG1, h1);
  element_set(tempG1_2, v);
  element_pow_zn(tempG1_2, tempG1_2, AliceSecondLevelParameter.k);
  element_mul(AliceSecondLevelCiphertext.c7, tempG1, tempG1_2);
  element_mul(AliceSecondLevelCiphertext.c7, AliceSecondLevelCiphertext.c7, w);
  element_pow_zn(AliceSecondLevelCiphertext.c7, AliceSecondLevelCiphertext.c7, AliceSecondLevelParameter.r);

  element_pow_zn(AliceSecondLevelCiphertext.c8, g, AliceSecondLevelParameter.n);  

  temp = H1(K, AliceSecondLevelCiphertext.c8, pairing);
  element_set_si(tempZR, temp);
  element_pow_zn(AliceSecondLevelCiphertext.c9, g, tempZR);
  // Enc1
  element_init_G1(tempG1, pairing);
  element_init_G1(tempG1_2, pairing);
  element_init_GT(tempGT, pairing);
  element_init_GT(tempGT_2, pairing);
  element_init_Zr(tempZR, pairing);


  element_random(AliceFirstLevelParameter.k);
  element_random(AliceFirstLevelParameter.r);
  element_random(AliceFirstLevelParameter.n);
  element_set(AliceFirstLevelCiphertext.c0, AliceFirstLevelParameter.k);
  element_pow_zn(AliceFirstLevelCiphertext.c1, d, AliceFirstLevelParameter.r);
  element_pairing(AliceFirstLevelCiphertext.c2, AlicePublicKey.part2, g);
  element_pow_zn(AliceFirstLevelCiphertext.c2, AliceFirstLevelCiphertext.c2, AliceFirstLevelParameter.r);


  element_pow_zn(K, Z, AliceFirstLevelParameter.r);
  element_pow_zn(tempG1, d, AliceFirstLevelParameter.r);
  temp = H1(K, tempG1, pairing);
  temp = temp ^ m;
  element_set_si(tempZR, temp);
  AliceFirstLevelCiphertext.c3.long_type = temp;
  element_pow_zn(AliceFirstLevelCiphertext.c3.G1_type, g, tempZR);

  element_set_si(h,H(AliceFirstLevelCiphertext.c1, AliceFirstLevelCiphertext.c3.G1_type, pairing));
  element_set(tempG1, u);
  element_pow_zn(tempG1, tempG1, h);
  element_set(tempG1_2, v);
  element_pow_zn(tempG1_2, tempG1_2, AliceFirstLevelParameter.k);
  element_mul(AliceFirstLevelCiphertext.c4, tempG1, tempG1_2);
  element_mul(AliceFirstLevelCiphertext.c4, AliceFirstLevelCiphertext.c4, w);
  element_pow_zn(AliceFirstLevelCiphertext.c4, AliceFirstLevelCiphertext.c4, AliceFirstLevelParameter.r);


  element_pow_zn(AliceFirstLevelCiphertext.c5, g, AliceFirstLevelParameter.n);
  temp = H1(K, AliceFirstLevelCiphertext.c5, pairing);
  element_set_si(tempZR, temp);
  element_pow_zn(AliceFirstLevelCiphertext.c6, g, tempZR);

  // Re-encrption
  element_init_G1(tempG1, pairing);
  element_init_G1(tempG1_2, pairing);
  element_init_GT(tempGT, pairing);
  element_init_GT(tempGT_2, pairing);
  element_init_Zr(tempZR, pairing);

  element_set(BobFirstLevelCiphertext.c1, AliceSecondLevelCiphertext.c1);
  element_set(BobFirstLevelCiphertext.c4, AliceSecondLevelCiphertext.c7);
  element_set(BobFirstLevelCiphertext.c5, AliceSecondLevelCiphertext.c8);
  element_set(BobFirstLevelCiphertext.c6, AliceSecondLevelCiphertext.c9);
  element_pairing(BobFirstLevelCiphertext.c2, AliceSecondLevelCiphertext.c2, r1);
  element_mul(tempG1, AlicePublicKey.part3[2], AlicePublicKey.part3[1]);
  element_pairing(tempGT, tempG1, AliceSecondLevelCiphertext.c4);
  element_mul(tempG1_2, r2, AlicePublicKey.part3[2]);
  element_pairing(tempGT_2, tempG1_2,  AliceSecondLevelCiphertext.c3);
  element_div(tempGT, tempGT, tempGT_2);
  temp = H1(tempGT, AliceSecondLevelCiphertext.c1, pairing);
  BobFirstLevelCiphertext.c3.long_type = AliceSecondLevelCiphertext.c5.long_type ^ temp;
  element_set_si(tempZR, temp);
  element_pow_zn(BobFirstLevelCiphertext.c3.G1_type, g, tempZR);
  // Dec2
  element_init_G1(tempG1, pairing);
  element_init_G1(tempG1_2, pairing);
  element_init_GT(tempGT, pairing);
  element_init_GT(tempGT_2, pairing);
  element_init_Zr(tempZR, pairing);
  

  element_pow_zn(tempZR, AliceSecretKey.a3, number[FILETYPES+1]);
  element_pow_zn(tempG1, g, tempZR);
  element_pairing(tempGT, tempG1, AliceSecondLevelCiphertext.c3);

  element_invert(tempZR, AliceSecretKey.a1);
  element_pairing(tempGT_2, AliceSecondLevelCiphertext.c2, g);
  element_pow_zn(tempGT_2, tempGT_2, tempZR);
  temp = AliceSecondLevelCiphertext.c5.long_type ^ H1(tempGT_2, AliceSecondLevelCiphertext.c1, pairing) ^ H1(tempGT, AliceSecondLevelCiphertext.c1, pairing);
  element_printf("m = %lu\n", temp);

 
  // Dec1
  element_init_G1(tempG1, pairing);
  element_init_G1(tempG1_2, pairing);
  element_init_GT(tempGT, pairing);
  element_init_GT(tempGT_2, pairing);
  element_init_Zr(tempZR, pairing);
  element_invert(tempZR, BobSecretKey.a1);
  element_pow_zn(tempGT, BobFirstLevelCiphertext.c2, tempZR);
  temp = H1(tempGT, BobFirstLevelCiphertext.c1, pairing) ^ BobFirstLevelCiphertext.c3.long_type;
  element_printf("m = %lu\n", temp);
// -------------------------------
  endTime = pbc_get_time();  
  printf("All time = %fs\n", endTime - beginTime);

  //free
  return 0;
}

unsigned long H1(element_t first, element_t second, pairing_t pairing)
{
  unsigned char *firstString = NULL;
  unsigned char *secondString = NULL;
  unsigned long firstHashValue = 0;
  unsigned long secondHashValue = 0;
  unsigned long ret = 0;
  int GTlength = 0;
  int G1length = 0;
  GTlength = pairing_length_in_bytes_G1(pairing);
  G1length = pairing_length_in_bytes_G1(pairing);
  firstString = malloc(GTlength);
  secondString = malloc(G1length);
  memset(firstString, 0, GTlength);
  memset(secondString, 0, G1length);
  if(firstString == NULL || secondString == NULL)
  {
    pbc_error("Memory allocation failed.");
    return 0;
  }
  if(element_to_bytes(firstString, first) <= 0 || element_to_bytes(secondString, second) <= 0)
  {
    pbc_error("element_to_bytes failed.");
    return 0;
  }
  firstString[GTlength] = '\0';
  secondString[G1length] = '\0';
  firstHashValue = hash(firstString);
  secondHashValue = hash(secondString);
  if(firstHashValue <= 0 || secondHashValue <= 0)
  {
    pbc_error("hash failed.");
    return 0;
  }
  ret = firstHashValue + secondHashValue;
  free(firstString);
  free(secondString);
  return ret;
}

unsigned long H(element_t first, element_t second, pairing_t pairing)
{
  unsigned char *firstString = NULL;
  unsigned char *secondString = NULL;
  unsigned long firstHashValue = 0;
  unsigned long secondHashValue = 0;
  unsigned long ret = 0;
  int length = 0;
  length = pairing_length_in_bytes_G1(pairing);
  firstString = malloc(length);
  secondString = malloc(length);
  memset(firstString, 0, length);
  memset(secondString, 0, length);
  if(firstString == NULL || secondString == NULL)
  {
    pbc_error("Memory allocation failed.");
    return 0;
  }
  if(element_to_bytes(firstString, first) <= 0 || element_to_bytes(secondString, second) <= 0)
  {
    pbc_error("element_to_bytes failed.");
    return 0;
  }
  firstString[length] = '\0';
  secondString[length] = '\0';
  firstHashValue = hash(firstString);
  secondHashValue = hash(secondString);
  if(firstHashValue <= 0 || secondHashValue <= 0)
  {
    pbc_error("hash failed.");
    return 0;
  }
  ret = firstHashValue + secondHashValue;
  free(firstString);
  free(secondString);
  return ret;
}

unsigned long hash(unsigned char *str)
{
  unsigned long hash = 5381;
  int c = 0;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}


