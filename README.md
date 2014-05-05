## cpp-metrics

cpp-metrics is a c++ implementation of measurement tools for tracking what's really happening in your code.
This library is richly inspired by codahale's [metrics] for the JVM.
If you aren't sold on this, watch this [talk on metrics].

## Things to do

* Document the project better
* Consider implementation of a gauge that caches its value for a period & a gauge interface
* A graphite pickle reporter
* Histogram
* Better reservoir implementations
* Remove all the lovely complicated language features to support older compilers

[metrics]: https://github.com/codahale/metrics
[talk on metrics]: https://www.youtube.com/watch?v=czes-oa0yik