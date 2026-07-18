# KyberSlash-writeup
## abstarct: 
With the recent advancements in quantum computing; there have been proposed proof of concepts that serve as evidence that the current cryptographic system could possibly be broken using a quantum computer in the future. This calls for rapid adaptation of various post quantum cryptography schemes that have been thoroughly tested to be implemented in the current encryption systems. One such proposed scheme is known as KyberSlash. With the fast adaptations and creations of such algorithms, there have been found vulnerabilities in this algorithm though patched; we shall attempt to understand the vulnerability and patch it ourselves in this report. We shall also attempt to recreate results from different write-ups and pre-existing papers.
```
grep -n "KYBER_Q" poly.c | grep "/"
```
