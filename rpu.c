#include <stdio.h>
#include <string.h>
#include "rsa.h"
#include "findopt.h"
#define strswitch if ( 0 ) { }
#define streq(n) else if(!strcmp(action, n))
#define error(s) fprintf(stderr, "Error: %s\n", s)
#define errordone(s) fprintf(stderr, "Exiting: %s\n", s); exit(1)
#define require(v) if ( v[0] == 'n' && v[1] == 0 ) { errordone("Required param: " # v); }
#define fsetfp(v, d, m) if ( !(v[0] == 'n' && v[1] == 0) ) { d = fopen(v, m); checkfile(d); }
#define checkfile(v) if ( v == NULL ) { errordone("Could not open file."); }
void keyreadfile(struct key_class *dest, char *filename) {
	FILE *fp = fopen(filename, "r");
	checkfile(fp);
	fscanf(fp, "%lld %lld", (long long*)&dest->modulus, (long long*)&dest->exponent);
	//printf("%lld %lld", dest->modulus, dest->exponent);
	fclose(fp);
}
// char *findopt(char *shorty, char *longy, int hasparam, int argc, char *argv[])
int main ( int argc, char **argv ) {
	struct public_key_class pub[1];
	struct private_key_class priv[1];
	char* infile = findopt("-f", "--file", 1, argc, argv);
	char* outfile = findopt("-o", "--out", 1, argc, argv);
	char* keyfile = findopt("-k", "--key", 1, argc, argv);
	char* privfile = findopt("-p", "--private", 1, argc, argv);
	char* action = findopt(argv[0], argv[0], 1, argc, argv);
	FILE *dest = stdout;
	FILE *src = stdin;
	fsetfp(infile, src, "r");
	fsetfp(outfile, dest, "w");
	if (0) { } streq("--help") {
		error("Usage: rsu [gen|encrypt|decrypt|sign|verify] [-f infile] [-o outfile] [-k pubkey] [-p privkey]");
		errordone("End usage.");
	} streq("gen") {
		require(keyfile);
		require(privfile);
		rsa_gen_keys(pub, priv, "primes.txt");
		FILE *privf = fopen(privfile, "w");
		checkfile(privf);
		fprintf(privf, "%lld %lld",(long long)priv->modulus,(long long)priv->exponent);
		fclose(privf);
		FILE *pubf = fopen(keyfile, "w");
		checkfile(pubf);
		fprintf(pubf, "%lld %lld",(long long)pub->modulus,(long long)pub->exponent);
		fclose(pubf);
	} streq("encrypt") {
		require(keyfile);
		keyreadfile(pub, keyfile);
		unsigned char *inbuf;
		int q = 0;
		inbuf = malloc(4096);
		while ( 1 ) {
		 q = fread(&inbuf[q], 1, 4096, src);
		 if ( feof(src) ) { break; }
		 inbuf = realloc(inbuf, q + 4096);
		}
	
		//printf("%d\n", q);
		long long *encrypted = rsa_encrypt(inbuf, q, pub);
		checkfile(encrypted);
		int p = 0;
		for (p = 0; p < q; p++) {
		fprintf(dest, "%lld ", (long long)encrypted[p]);
		}
		
	} streq("decrypt") {
		require(privfile);
		keyreadfile(priv, privfile);
		long long *encrypted = malloc(sizeof(long long));
		int q = 0;
		while ( 1 ) {
		// printf("%d\n", q);
		 fscanf(src, "%lld ", &encrypted[q]);
		 if ( feof(src) ) { break; }
		 q++;
		 encrypted = realloc(encrypted, (q+4)*sizeof(long long));
		}
		//printf("%lld", encrypted[0]);
		unsigned char* decrypted = rsa_decrypt(encrypted, q*sizeof(long long), priv);
		checkfile(decrypted);
		fwrite(decrypted, 1, q, dest);
	} streq("sign") {
		require(privfile);
		keyreadfile(pub, privfile);
		unsigned char *inbuf;
		int q = 0;
		inbuf = malloc(4096);
		while ( 1 ) {
		 q = fread(&inbuf[q], 1, 4096, src);
		 if ( feof(src) ) { break; }
		 inbuf = realloc(inbuf, q + 4096);
		}
	
		//printf("%d\n", q);
		char *dat123 = str_crc32(inbuf, q);
		//printf("Hi %s\n", dat123);
		long long *encrypted = rsa_encrypt(dat123, 9, pub);
		checkfile(encrypted);
		int p = 0;
		for (p = 0; p < 9; p++) {
		fprintf(dest, "%lld ", (long long)encrypted[p]);
		}
	} streq("verify") {
		require(keyfile);
		keyreadfile(priv, keyfile);
		long long *encrypted = malloc(sizeof(long long));
		int q = 0;
		while ( 1 ) {
		// printf("%d\n", q);
		 fscanf(src, "%lld ", &encrypted[q]);
		 if ( feof(src) ) { break; }
		 q++;
		 encrypted = realloc(encrypted, (q+4)*sizeof(long long));
		}
		//printf("%lld", encrypted[0]);
		unsigned char* decrypted = rsa_decrypt(encrypted, q*sizeof(long long), priv);
		checkfile(decrypted);
		fwrite(decrypted, 1, q, dest);
	}
	return 0;
}