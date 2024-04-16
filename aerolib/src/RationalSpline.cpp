#include "RationalSpline.h"
#include <algorithm>
#include <cmath>

RationalSpline::RationalSpline(const std::vector<double>& x, const std::vector<double>& y) {
    int N = (int)x.size();
    m_P.resize(N, 0.001);
    m_Q.resize(N, 0.001);
    create(x, y);
}

RationalSpline::RationalSpline(const std::vector<double>& x, const std::vector<double>& y, double GlobalWeight) {
    int N = (int)x.size();
    m_P.resize(N, std::max(0.001, std::min(GlobalWeight, 100.0)));
    m_Q.resize(N, std::max(0.001, std::min(GlobalWeight, 100.0)));
    create(x, y);
}

RationalSpline::RationalSpline(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& w) {
    int N = (int)x.size();
    m_P.resize(N);
    m_Q.resize(N);

    for (int i = 0; i < N; ++i) {
        m_P[i] = m_Q[i] = std::max(0.001, std::min(w[i], 100.0));
    }

    create(x, y);
}

void RationalSpline::create(const std::vector<double>& x, const std::vector<double>& y) {
    m_N = (int)x.size();
    m_Y1.resize(m_N);
    m_X = x;
    m_Y = y;
    m_A.resize(m_N - 1);
    m_B.resize(m_N - 1);
    m_C.resize(m_N - 1);
    m_D.resize(m_N - 1);

    for (int k = 0; k < m_N; ++k) {
        m_X[k] = x[k];
        m_Y[k] = y[k];
    }

    int N1 = m_N - 1;
    int N2 = N1 - 1;
    m_Y1[0] = (m_Y[1] - m_Y[0]) / (m_X[1] - m_X[0]);
    m_Y1[N1] = (m_Y[N1] - m_Y[N2]) / (m_X[N1] - m_X[N2]);
    m_C[0] = 0.0;
    m_D[0] = 0.0;
    int J1 = 0;
    double QQ1 = 0.0;
    double P21 = 0.0;
    double R1 = 0.0;
    double H1 = 0.0;

    double H;
    int k1;
    double Z;
    double P2;
    double Q2;
    for (int k = 0; k < N1; k = k1) {
        k1 = k + 1;
        Z = m_P[k];
        P2 = m_Q[k];
        Q2 = Z * (Z + 3.0) + 3.0;
        double QQ2 = P2 * (P2 + 3.0) + 3.0;
        double P22 = 2.0 + Z;
        double Q22 = 2.0 + P2;
        m_A[k] = m_X[k1] - m_X[k];
        H = 1.0 / m_A[k];
        m_B[k] = 1.0 / (P22 * Q22 - 1.0);
        double H2 = H * m_B[k];
        double R2 = H * H2 * (m_Y[k1] - m_Y[k]);
        if (k > 0) {
            double HQ = H1 * QQ1;
            double HP = H2 * Q2;
            Z = 1.0 / (HQ * (P21 - m_C[J1]) + HP * Q22);
            m_C[k] = Z * HP;
            H = R1 * QQ1 * (1.0 + P21) + R2 * Q2 * (1.0 + Q22);
            if (k == 1) {
                H -= HQ * m_Y1[0];
            }
            else if (k == N2) {
                H -= HP * m_Y1[N1];
            }
            m_D[k] = Z * (H - HQ * m_D[J1]);
        }
        J1 = k;
        P21 = P22;
        QQ1 = QQ2;
        H1 = H2;
        R1 = R2;
    }

    m_Y1[N2] = m_D[N2];
    if (N1 > 2) {
        for (J1 = 1; J1 < N2; ++J1) {
            int k = N2 - J1;
            m_Y1[k] = m_D[k] - m_C[k] * m_Y1[k + 1];
        }
    }

    for (int k = 0; k < N1; k = k1) {
        k1 = k + 1;
        H = m_B[k] * (m_Y[k1] - m_Y[k]);
        Z = m_B[k] * m_A[k];
        P2 = 2.0 + m_P[k];
        Q2 = 2.0 + m_Q[k];
        m_C[k] = (1.0 + Q2) * H - Z * (m_Y1[k1] + Q2 * m_Y1[k]);
        m_D[k] = -(1.0 + P2) * H + Z * (P2 * m_Y1[k1] + m_Y1[k]);
        m_A[k] = m_Y[k] - m_C[k];
        m_B[k] = m_Y[k1] - m_D[k];
    }
}

double RationalSpline::EvalPoint(double x) const {
    double dRetVal = 0.0;
    x = std::max(x, m_X[0]);
    x = std::min(x, m_X[m_N - 1]);

    for (int k = 0; k < m_N - 1; ++k) {
        if (x >= m_X[k] && x <= m_X[k + 1]) {
            double dV = (x - m_X[k]) / (m_X[k + 1] - m_X[k]);
            double dU = 1.0 - dV;
            dRetVal = m_A[k] * dU + m_B[k] * dV + m_C[k] * phi(dU, dV, m_P[k]) + m_D[k] * phi(dV, dU, m_Q[k]);
            break;
        }
    }

    return dRetVal;
}

const std::vector<double>& RationalSpline::getA() const {
    return m_A;
}

const std::vector<double>& RationalSpline::getB() const {
    return m_B;
}

const std::vector<double>& RationalSpline::getC() const {
    return m_C;
}

const std::vector<double>& RationalSpline::getD() const {
    return m_D;
}

double RationalSpline::phi(double U, double V, double P) const {
    double RetVal = std::pow(U, 3.0) / (P * V + 1.0);
    return RetVal;
}
