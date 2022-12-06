# asdasd

例如，计算函数 $f(x) = x^3$ 在区间 $[1, 2]$ 内的定积分，可以使用梯形法，将区间 $[1, 2]$ 划分为 $n$ 个子区间，每个子区间的宽度为 $h=\frac{2-1}{n}=\frac{1}{n}$，则区间 $[1, 2]$ 内的定积分值为：

$$\begin{aligned} \int_{1}^{2} f(x) , dx &= \lim_{n \to \infty} \sum_{i=1}^{n} \frac{1}{n} \cdot \frac{1}{3} \left(x_{i}^3 + x_{i-1}^3\right) \ &= \lim_{n \to \infty} \frac{1}{3} \cdot \frac{1}{n} \sum_{i=1}^{n} \left(x_{i}^3 + x_{i-1}^3\right) \ &= \lim_{n \to \infty} \frac{1}{3} \cdot \frac{1}{n} \cdot \left(x_0^3 + x_1^3 + \dots + x_n^3\right) \ &= \lim_{n \to \infty} \frac{1}{3} \cdot \frac{1}{n} \cdot \left(1^3 + \frac{1}{n}^3 + \frac{2}{n}^3 + \dots + \frac{n}{n}^3\right) \ &= \lim_{n \to \infty} \frac{1}{3} \cdot \frac{1}{n} \cdot \left(1 + \frac{1}{n^3} + \frac{8}{n^3}
