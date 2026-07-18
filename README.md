# KyberSlash-writeup
## abstract:  
With the recent advancements in quantum computing; there have been proposed proof of concepts that serve as evidence that the current cryptographic system could possibly be broken using a quantum computer in the future. This calls for rapid adaptation of various post quantum cryptography schemes that have been thoroughly tested to be implemented in the current encryption systems. One such proposed scheme is known as Kyber. With the fast adaptations and creations of such algorithms, there have been found vulnerabilities in this algorithm though patched; we shall attempt to understand the vulnerability and patch it ourselves in this report. We shall also attempt to recreate results from different write-ups and pre-existing papers.

### Introduction:

We can intuitively understand Kyber as a post quantum protocol that was being used in order to transfer keys , encapsulate and decapsulate keys; very similar to public and private key exchange in the current RSA system being used. Currently though the protocol is actually known as ML-KEM; the algorithm is essentially based on learning with errors and module learning with errors. The main difference between either of them being the fact that learning with error makes use of vectors and matrices of integers while module learning with errors makes use of vectors and matrices of polynomials. 

One of the main focuses of the project shall be to understand where the vulnerability came from and one can go about patching this sort of thing. Though many of the papers written on this vulnerability specifically do make use of external hardware but we shall make up for this by sampling around 1000s of values to reduce as much of the noise as possible. 

### Methodology:
To begin we must install the vulnerable code onto our systems though I'd suggest not worrying about the code doing some sort of damage to our system since it is in fact vulnerable but we just want to patch that part of the code rather than use it for actual key transfer. So what is going on here? What is KyberSlash?

KyberSlash refers to one of the timing attacks that were discovered in the adapted post quantum Kyber algorithm though now patched. We can clone the repo onto our systems and after installation we shall make use of the following bash commands in order to find the vulnerability. 

1. we shall go back to the time before the vulnerability was patched:
```bash
git log --oneline --before="2023-12-01" -5
```
2. Look for the specific code where the vulnerability existed:
```bash
grep -n "KYBER_Q" poly.c | grep "/"
```
3. Open the file directly:

```bash
grep -n -A 5 "poly_tomsg" poly.c
```
ML-KEM offers parameter sets designed for NIST security levels 1, 3, and 5. For each security level, it uses fixed parameters q = 3329 and n = 256 that define the polynomial ring $\R_q = \Z_q[x]/ (x_n + 1)$, over which most of the operations are performed.

### Kyber Encoding and Decoding

Kyber uses an encoding procedure that allows it to recover the message even after some noise accumulates during encryption and decryption.

It encodes a 256-bit message $m \in \mathbb{Z}_2^{256}$ into a polynomial in $R_q$ as:

$$
\text{Encode}(m) = m_0 + m_1 x + \cdots + m_{n-1} x^{n-1} \in R_q
$$

where:

$$
m_i = m[i] \cdot \left\lceil \frac{q}{2} \right\rceil
$$

In other words, if bit $m[i] = 0$, then $m_i = 0$; otherwise $m_i = \left\lceil \dfrac{q}{2} \right\rceil$.

A simple decoding procedure can then be applied to a polynomial $m'$. Namely, the decoding function outputs a 256-bit message:

$$
m = \text{Decode}(m')
$$

from a noisy polynomial $m'$ by simply checking if each coefficient of $m'$ is closer to $0$ or to $\dfrac{q}{2}$, modulo $q$, and decoding it to $0$ or $1$, correspondingly.

Here's the critical line:
```c
t  = (((t << 1) + KYBER_Q/2)/KYBER_Q) & 1;
```
