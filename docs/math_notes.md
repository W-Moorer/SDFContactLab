# Math Notes

This scaffold adopts the paper convention:

- positive SDF outside the body;
- zero SDF on the surface;
- negative SDF inside the body.

The current implementation includes analytic body-frame SDFs for:

- sphere;
- ellipsoid;
- axis-aligned box;
- plane.

World-space queries are handled by transforming the query point into the body frame, evaluating the local SDF, and rotating the gradient back to world coordinates.

The contact pipeline in this first skeleton is not yet the full bidirectional projection and clustering method from the paper. It provides a stable placeholder interface so higher-level repository pieces can already compile and exchange data.
