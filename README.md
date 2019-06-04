
In our proposed scheme, we integrate a [PRE scheme](https://link.springer.com/article/10.1007/s11432-010-0047-3) and a [key-aggregate approach](https://ieeexplore.ieee.org/document/6497048/) into a key-aggregate proxy re-encryption scheme. The proposed algorithm handles performance issues in reducing the number of re-keys while achieving fine-grained access control on the encrypted files, simultaneously.


### How to compile 
```
1.gcc -o <outputname> <filename> -I /usr/local/include/pbc -L /usr/local/lib -Wl,-rpath /usr/local/lib  -lpbc -lgmp
2. ./<outputname> param/<paramfile>
```

## Built With
* [M4](http://www.gnu.org/software/m4/m4.html) - GNU M4 is an implementation of the traditional Unix macro processor.
* [Flex](https://github.com/westes/flex) - Flex is a tool for generating scanners.
* [Bison](https://www.gnu.org/software/bison/) - Bison is a general-purpose parser generator.
* [GMP](https://gmplib.org/) - The GNU Multiple Precision Arithmetic Library.
* [PBC](https://crypto.stanford.edu/pbc/) - The Pairing-Based Cryptography Library.

Build Flow:

![Install flow](https://github.com/ferranschen/KAPRE/blob/master/Install%20flow.png)

## Authors

* [**Ferrans Chen**](https://github.com/ferranschen)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details




