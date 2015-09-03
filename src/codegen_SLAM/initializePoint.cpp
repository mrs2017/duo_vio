//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: initializePoint.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 03-Sep-2015 16:53:59
//

// Include Files
#include "rt_nonfinite.h"
#include "SLAM.h"
#include "initializePoint.h"
#include "norm.h"
#include "svd.h"
#include "get_r_u.h"
#include "QuatFromRotJ.h"
#include "SLAM_rtwutil.h"
#include <ros/console.h>
#include <stdio.h>

// Function Declarations
static void b_eml_xgeqp3(double A[30], double tau[5], int jpvt[5]);
static double f_eml_xnrm2(int n, const double x[30], int ix0);

// Function Definitions

//
// Arguments    : double A[30]
//                double tau[5]
//                int jpvt[5]
// Return Type  : void
//
static void b_eml_xgeqp3(double A[30], double tau[5], int jpvt[5])
{
  double work[5];
  int i22;
  double vn1[5];
  double vn2[5];
  int k;
  int iy;
  double smax;
  double temp2;
  int itemp;
  double absxk;
  double t;
  int i;
  int i_i;
  int ix;
  int pvt;
  int i_ip1;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int32_T exitg1;
  for (i22 = 0; i22 < 5; i22++) {
    jpvt[i22] = 1 + i22;
    work[i22] = 0.0;
  }

  k = 1;
  for (iy = 0; iy < 5; iy++) {
    smax = 0.0;
    temp2 = 2.2250738585072014E-308;
    for (itemp = k; itemp <= k + 5; itemp++) {
      absxk = fabs(A[itemp - 1]);
      if (absxk > temp2) {
        t = temp2 / absxk;
        smax = 1.0 + smax * t * t;
        temp2 = absxk;
      } else {
        t = absxk / temp2;
        smax += t * t;
      }
    }

    smax = temp2 * sqrt(smax);
    vn1[iy] = smax;
    vn2[iy] = vn1[iy];
    k += 6;
  }

  for (i = 0; i < 5; i++) {
    i_i = i + i * 6;
    itemp = 0;
    if (5 - i > 1) {
      ix = i;
      smax = fabs(vn1[i]);
      for (k = 1; k + 1 <= 5 - i; k++) {
        ix++;
        temp2 = fabs(vn1[ix]);
        if (temp2 > smax) {
          itemp = k;
          smax = temp2;
        }
      }
    }

    pvt = i + itemp;
    if (pvt + 1 != i + 1) {
      ix = 6 * pvt;
      iy = 6 * i;
      for (k = 0; k < 6; k++) {
        smax = A[ix];
        A[ix] = A[iy];
        A[iy] = smax;
        ix++;
        iy++;
      }

      itemp = jpvt[pvt];
      jpvt[pvt] = jpvt[i];
      jpvt[i] = itemp;
      vn1[pvt] = vn1[i];
      vn2[pvt] = vn2[i];
    }

    absxk = A[i_i];
    temp2 = 0.0;
    smax = f_eml_xnrm2(5 - i, A, i_i + 2);
    if (smax != 0.0) {
      smax = rt_hypotd_snf(A[i_i], smax);
      if (A[i_i] >= 0.0) {
        smax = -smax;
      }

      if (fabs(smax) < 1.0020841800044864E-292) {
        itemp = 0;
        do {
          itemp++;
          i22 = i_i - i;
          for (k = i_i + 1; k + 1 <= i22 + 6; k++) {
            A[k] *= 9.9792015476736E+291;
          }

          smax *= 9.9792015476736E+291;
          absxk *= 9.9792015476736E+291;
        } while (!(fabs(smax) >= 1.0020841800044864E-292));

        smax = rt_hypotd_snf(absxk, f_eml_xnrm2(5 - i, A, i_i + 2));
        if (absxk >= 0.0) {
          smax = -smax;
        }

        temp2 = (smax - absxk) / smax;
        absxk = 1.0 / (absxk - smax);
        i22 = i_i - i;
        for (k = i_i + 1; k + 1 <= i22 + 6; k++) {
          A[k] *= absxk;
        }

        for (k = 1; k <= itemp; k++) {
          smax *= 1.0020841800044864E-292;
        }

        absxk = smax;
      } else {
        temp2 = (smax - A[i_i]) / smax;
        absxk = 1.0 / (A[i_i] - smax);
        i22 = i_i - i;
        for (k = i_i + 1; k + 1 <= i22 + 6; k++) {
          A[k] *= absxk;
        }

        absxk = smax;
      }
    }

    tau[i] = temp2;
    A[i_i] = absxk;
    if (i + 1 < 5) {
      absxk = A[i_i];
      A[i_i] = 1.0;
      i_ip1 = (i + (i + 1) * 6) + 1;
      if (tau[i] != 0.0) {
        lastv = 6 - i;
        itemp = i_i - i;
        while ((lastv > 0) && (A[itemp + 5] == 0.0)) {
          lastv--;
          itemp--;
        }

        lastc = 4 - i;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          itemp = i_ip1 + (lastc - 1) * 6;
          k = itemp;
          do {
            exitg1 = 0;
            if (k <= (itemp + lastv) - 1) {
              if (A[k - 1] != 0.0) {
                exitg1 = 1;
              } else {
                k++;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        lastc = 0;
      }

      if (lastv > 0) {
        if (lastc == 0) {
        } else {
          for (iy = 1; iy <= lastc; iy++) {
            work[iy - 1] = 0.0;
          }

          iy = 0;
          i22 = i_ip1 + 6 * (lastc - 1);
          for (itemp = i_ip1; itemp <= i22; itemp += 6) {
            ix = i_i;
            smax = 0.0;
            pvt = (itemp + lastv) - 1;
            for (k = itemp; k <= pvt; k++) {
              smax += A[k - 1] * A[ix];
              ix++;
            }

            work[iy] += smax;
            iy++;
          }
        }

        if (-tau[i] == 0.0) {
        } else {
          itemp = i_ip1 - 1;
          pvt = 0;
          for (iy = 1; iy <= lastc; iy++) {
            if (work[pvt] != 0.0) {
              smax = work[pvt] * -tau[i];
              ix = i_i;
              i22 = lastv + itemp;
              for (k = itemp; k + 1 <= i22; k++) {
                A[k] += A[ix] * smax;
                ix++;
              }
            }

            pvt++;
            itemp += 6;
          }
        }
      }

      A[i_i] = absxk;
    }

    for (iy = i + 1; iy + 1 < 6; iy++) {
      if (vn1[iy] != 0.0) {
        smax = fabs(A[i + 6 * iy]) / vn1[iy];
        smax = 1.0 - smax * smax;
        if (smax < 0.0) {
          smax = 0.0;
        }

        temp2 = vn1[iy] / vn2[iy];
        temp2 = smax * (temp2 * temp2);
        if (temp2 <= 1.4901161193847656E-8) {
          vn1[iy] = d_eml_xnrm2(5 - i, A, (i + 6 * iy) + 2);
          vn2[iy] = vn1[iy];
        } else {
          vn1[iy] *= sqrt(smax);
        }
      }
    }
  }
}

//
// Arguments    : int n
//                const double x[30]
//                int ix0
// Return Type  : double
//
static double f_eml_xnrm2(int n, const double x[30], int ix0)
{
  double y;
  double scale;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n == 1) {
    y = fabs(x[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fabs(x[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

//
// Arguments    : int n
//                const double x[30]
//                int ix0
// Return Type  : double
//
double d_eml_xnrm2(int n, const double x[30], int ix0)
{
  double y;
  double scale;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n == 1) {
    y = fabs(x[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fabs(x[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

//
// INITIALIZEPOINT Initialize a feature point from current measurements
//
//  INPUT ARGUMENTS:
//  - xt: The current camera state
//  - cameraparams: The camera parameter struct
//  - z_l: The measurement of the feature in the left camera
//  - z_r: The measurement of the feature in the right camera
//
//  OUTPUT ARGUMENTS:
//  - fp: The feature point in the world frame
//  - m:  The ray in the left camera frame to the feature
// Arguments    : const emxArray_real_T *b_xt
//                const double c_cameraparams_CameraParameters[3]
//                const double d_cameraparams_CameraParameters[2]
//                const double e_cameraparams_CameraParameters[2]
//                const double f_cameraparams_CameraParameters[3]
//                const double g_cameraparams_CameraParameters[2]
//                const double h_cameraparams_CameraParameters[2]
//                const double cameraparams_r_lr[3]
//                const double cameraparams_R_lr[9]
//                const double z_l[2]
//                const double z_r[2]
//                double fp[3]
//                double m_out[3]
// Return Type  : void
//
void initializePoint(const emxArray_real_T *b_xt, const double
                     c_cameraparams_CameraParameters[3], const double
                     d_cameraparams_CameraParameters[2], const double
                     e_cameraparams_CameraParameters[2], const double
                     f_cameraparams_CameraParameters[3], const double
                     g_cameraparams_CameraParameters[2], const double
                     h_cameraparams_CameraParameters[2], const double
                     cameraparams_r_lr[3], const double cameraparams_R_lr[9],
                     const double z_l[2], const double z_r[2], double fp[3],
                     double m_out[3])
{
  double b_cameraparams_R_lr[9];
  double pos[6];
  int rankR;
  int i;
  double dv2[4];
  double rot[8];
  static const signed char iv3[4] = { 0, 0, 0, 1 };

  double zn_d_l[2];
  double zn_d_r[2];
  double r_u_l;
  double r_u_r;
  double b_r_u_l[2];
  boolean_T b[2];
  boolean_T y;
  boolean_T exitg2;
  boolean_T guard1 = false;
  double c_r_u_l[2];
  boolean_T exitg1;
  double B;
  double tol;
  double mr[3];
  double m[6];
  double A[30];
  double b_b[6];
  int j;
  int anchorIdx;
  double b_rot[9];
  signed char I[9];
  double unusedExpr[5];
  int jpvt[5];
  double tau[5];
  double x[5];
  double c_xt[9];

  //  camera parameters for the left and right camera
  //  if ~all(size(q) == [4, 1])
  //      error('q does not have the size of a quaternion')
  //  end
  //  if abs(norm(q) - 1) > 1e-3
  //      error('The provided quaternion is not a valid rotation quaternion because it does not have norm 1') 
  //  end
  //  R_lw = R_cw;
  //  R_rw = R_lr'*R_cw;
  //  pos  = [r_cw,r_cw + R_cw*r_lr];
  //  rot  = [QuatFromRotJ(R_lw),QuatFromRotJ(R_rw)];
  for (rankR = 0; rankR < 3; rankR++) {
    pos[rankR] = 0.0;
    pos[3 + rankR] = cameraparams_r_lr[rankR];
    for (i = 0; i < 3; i++) {
      b_cameraparams_R_lr[i + 3 * rankR] = cameraparams_R_lr[rankR + 3 * i];
    }
  }

  QuatFromRotJ(b_cameraparams_R_lr, dv2);
  for (rankR = 0; rankR < 4; rankR++) {
    rot[rankR] = iv3[rankR];
    rot[4 + rankR] = dv2[rankR];
  }

  zn_d_l[0] = (z_l[0] - e_cameraparams_CameraParameters[0]) /
    d_cameraparams_CameraParameters[0];
  zn_d_l[1] = (z_l[1] - e_cameraparams_CameraParameters[1]) /
    d_cameraparams_CameraParameters[1];
  zn_d_r[0] = (z_r[0] - h_cameraparams_CameraParameters[0]) /
    g_cameraparams_CameraParameters[0];
  zn_d_r[1] = (z_r[1] - h_cameraparams_CameraParameters[1]) /
    g_cameraparams_CameraParameters[1];

  //  the radius for the undistortion
  r_u_l = get_r_u(c_cameraparams_CameraParameters[0],
                  c_cameraparams_CameraParameters[1],
                  c_cameraparams_CameraParameters[2], sqrt(zn_d_l[0] * zn_d_l[0]
    + zn_d_l[1] * zn_d_l[1]));
  r_u_r = get_r_u(f_cameraparams_CameraParameters[0],
                  f_cameraparams_CameraParameters[1],
                  f_cameraparams_CameraParameters[2], sqrt(zn_d_r[0] * zn_d_r[0]
    + zn_d_r[1] * zn_d_r[1]));
  b_r_u_l[0] = r_u_l;
  b_r_u_l[1] = r_u_r;
  for (rankR = 0; rankR < 2; rankR++) {
    b[rankR] = rtIsNaN(b_r_u_l[rankR]);
  }

  y = false;
  i = 0;
  exitg2 = false;
  while ((!exitg2) && (i < 2)) {
    if (!!b[i]) {
      y = true;
      exitg2 = true;
    } else {
      i++;
    }
  }

  guard1 = false;
  if (!y) {
    c_r_u_l[0] = r_u_l;
    c_r_u_l[1] = r_u_r;
    for (rankR = 0; rankR < 2; rankR++) {
      b[rankR] = rtIsInf(c_r_u_l[rankR]);
    }

    y = false;
    i = 0;
    exitg1 = false;
    while ((!exitg1) && (i < 2)) {
      if (!!b[i]) {
        y = true;
        exitg1 = true;
      } else {
        i++;
      }
    }

    if (!y) {
      B = ((1.0 + c_cameraparams_CameraParameters[0] * (r_u_l * r_u_l)) +
           c_cameraparams_CameraParameters[1] * rt_powd_snf(r_u_l, 4.0)) +
        c_cameraparams_CameraParameters[2] * rt_powd_snf(r_u_l, 6.0);

      // undistort points
      tol = ((1.0 + f_cameraparams_CameraParameters[0] * (r_u_r * r_u_r)) +
             f_cameraparams_CameraParameters[1] * rt_powd_snf(r_u_r, 4.0)) +
        f_cameraparams_CameraParameters[2] * rt_powd_snf(r_u_r, 6.0);
      for (rankR = 0; rankR < 2; rankR++) {
        zn_d_l[rankR] /= B;
        zn_d_r[rankR] /= tol;
      }

      m_out[0] = zn_d_l[0];
      m_out[1] = zn_d_l[1];
      m_out[2] = 1.0;
      mr[0] = zn_d_r[0];
      mr[1] = zn_d_r[1];
      mr[2] = 1.0;
      B = norm(m_out);
      tol = norm(mr);
      for (rankR = 0; rankR < 3; rankR++) {
        m[rankR] = m_out[rankR] / B;
        m[3 + rankR] = mr[rankR] / tol;
      }

      // TRIANGULATIONN Triangulate a point from several measurements
      //    Calculate the world position of a point given measurements of it from 
      //    different positions. The solution minimizes the least squares error
      //
      //  INPUTS:
      //  pos: a 3 x n matrix containing the positions of the cameras
      //  rot: a 4 x n matrix containing the JPL quaternions of the cameras
      //  m: a 3 x n matrix containing the unit vectors pointing towards the
      //      feature in the camera frame
      //  where n is the number of poses
      //
      //  OUTPUTS:
      //  pf: the feature position in world coordinates
      //  condition: the condition number of the least squares problem. High
      //      numbers mean a badly conditiond problem and a bad result
      memset(&A[0], 0, 30U * sizeof(double));
      for (j = 0; j < 2; j++) {
        //  if ~all(size(q) == [4, 1])
        //      error('q does not have the size of a quaternion')
        //  end
        //  if abs(norm(q) - 1) > 1e-3
        //      error('The provided quaternion is not a valid rotation quaternion because it does not have norm 1') 
        //  end
        anchorIdx = j * 3;
        b_rot[0] = ((rot[j << 2] * rot[j << 2] - rot[1 + (j << 2)] * rot[1 + (j <<
          2)]) - rot[2 + (j << 2)] * rot[2 + (j << 2)]) + rot[3 + (j << 2)] *
          rot[3 + (j << 2)];
        b_rot[1] = 2.0 * (rot[j << 2] * rot[1 + (j << 2)] + rot[2 + (j << 2)] *
                          rot[3 + (j << 2)]);
        b_rot[2] = 2.0 * (rot[j << 2] * rot[2 + (j << 2)] - rot[1 + (j << 2)] *
                          rot[3 + (j << 2)]);
        b_rot[3] = 2.0 * (rot[j << 2] * rot[1 + (j << 2)] - rot[2 + (j << 2)] *
                          rot[3 + (j << 2)]);
        b_rot[4] = ((-(rot[j << 2] * rot[j << 2]) + rot[1 + (j << 2)] * rot[1 +
                     (j << 2)]) - rot[2 + (j << 2)] * rot[2 + (j << 2)]) + rot[3
          + (j << 2)] * rot[3 + (j << 2)];
        b_rot[5] = 2.0 * (rot[1 + (j << 2)] * rot[2 + (j << 2)] + rot[j << 2] *
                          rot[3 + (j << 2)]);
        b_rot[6] = 2.0 * (rot[j << 2] * rot[2 + (j << 2)] + rot[1 + (j << 2)] *
                          rot[3 + (j << 2)]);
        b_rot[7] = 2.0 * (rot[1 + (j << 2)] * rot[2 + (j << 2)] - rot[j << 2] *
                          rot[3 + (j << 2)]);
        b_rot[8] = ((-(rot[j << 2] * rot[j << 2]) - rot[1 + (j << 2)] * rot[1 +
                     (j << 2)]) + rot[2 + (j << 2)] * rot[2 + (j << 2)]) + rot[3
          + (j << 2)] * rot[3 + (j << 2)];
        for (rankR = 0; rankR < 3; rankR++) {
          A[(rankR + anchorIdx) + 6 * j] = 0.0;
          for (i = 0; i < 3; i++) {
            A[(rankR + anchorIdx) + 6 * j] += b_rot[rankR + 3 * i] * m[i + 3 * j];
          }
        }

        for (rankR = 0; rankR < 9; rankR++) {
          I[rankR] = 0;
        }

        anchorIdx = j * 3;
        for (i = 0; i < 3; i++) {
          I[i + 3 * i] = 1;
          for (rankR = 0; rankR < 3; rankR++) {
            A[(rankR + anchorIdx) + 6 * (2 + i)] = -(double)I[rankR + 3 * i];
          }
        }

        anchorIdx = j * 3;
        for (rankR = 0; rankR < 3; rankR++) {
          b_b[rankR + anchorIdx] = -pos[rankR + 3 * j];
        }
      }

      svd(A, unusedExpr);
      b_eml_xgeqp3(A, tau, jpvt);
      rankR = 0;
      tol = 6.0 * fabs(A[0]) * 2.2204460492503131E-16;
      while ((rankR < 5) && (fabs(A[rankR + 6 * rankR]) >= tol)) {
        rankR++;
      }

      for (i = 0; i < 5; i++) {
        x[i] = 0.0;
      }

      for (j = 0; j < 5; j++) {
        if (tau[j] != 0.0) {
          tol = b_b[j];
          for (i = j + 1; i + 1 < 7; i++) {
            tol += A[i + 6 * j] * b_b[i];
          }

          tol *= tau[j];
          if (tol != 0.0) {
            b_b[j] -= tol;
            for (i = j + 1; i + 1 < 7; i++) {
              b_b[i] -= A[i + 6 * j] * tol;
            }
          }
        }
      }

      for (i = 0; i + 1 <= rankR; i++) {
        x[jpvt[i] - 1] = b_b[i];
      }

      for (j = rankR - 1; j + 1 > 0; j--) {
        x[jpvt[j] - 1] /= A[j + 6 * j];
        for (i = 0; i + 1 <= j; i++) {
          x[jpvt[i] - 1] -= x[jpvt[j] - 1] * A[i + 6 * j];
        }
      }

      // transform to world coordinates
      c_xt[0] = ((b_xt->data[3] * b_xt->data[3] - b_xt->data[4] * b_xt->data[4])
                 - b_xt->data[5] * b_xt->data[5]) + b_xt->data[6] * b_xt->data[6];
      c_xt[1] = 2.0 * (b_xt->data[3] * b_xt->data[4] + b_xt->data[5] *
                       b_xt->data[6]);
      c_xt[2] = 2.0 * (b_xt->data[3] * b_xt->data[5] - b_xt->data[4] *
                       b_xt->data[6]);
      c_xt[3] = 2.0 * (b_xt->data[3] * b_xt->data[4] - b_xt->data[5] *
                       b_xt->data[6]);
      c_xt[4] = ((-(b_xt->data[3] * b_xt->data[3]) + b_xt->data[4] * b_xt->data
                  [4]) - b_xt->data[5] * b_xt->data[5]) + b_xt->data[6] *
        b_xt->data[6];
      c_xt[5] = 2.0 * (b_xt->data[4] * b_xt->data[5] + b_xt->data[3] *
                       b_xt->data[6]);
      c_xt[6] = 2.0 * (b_xt->data[3] * b_xt->data[5] + b_xt->data[4] *
                       b_xt->data[6]);
      c_xt[7] = 2.0 * (b_xt->data[4] * b_xt->data[5] - b_xt->data[3] *
                       b_xt->data[6]);
      c_xt[8] = ((-(b_xt->data[3] * b_xt->data[3]) - b_xt->data[4] * b_xt->data
                  [4]) + b_xt->data[5] * b_xt->data[5]) + b_xt->data[6] *
        b_xt->data[6];
      for (rankR = 0; rankR < 3; rankR++) {
        tol = 0.0;
        for (i = 0; i < 3; i++) {
          tol += c_xt[rankR + 3 * i] * x[2 + i];
        }

        fp[rankR] = tol + b_xt->data[rankR];
      }

      B = norm(m_out);
      for (rankR = 0; rankR < 3; rankR++) {
        m_out[rankR] /= B;
      }
    } else {
      guard1 = true;
    }
  } else {
    guard1 = true;
  }

  if (guard1) {
    for (i = 0; i < 3; i++) {
      fp[i] = rtNaN;
      m_out[i] = rtNaN;
    }
  }
}

//
// File trailer for initializePoint.cpp
//
// [EOF]
//
