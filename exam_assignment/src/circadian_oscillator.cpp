#include "types.cpp"

vessel_t circadian_oscillator()
{
    auto alphaA = 50.0;
    auto alpha_A = 500.0;
    auto alphaR = 0.01;
    auto alpha_R = 50.0;
    auto betaA = 50.0;
    auto betaR = 5.0;
    auto gammaA = 1.0;
    auto gammaR = 1.0;
    auto gammaC = 2.0;
    auto deltaA = 1.0;
    auto deltaR = 0.2;
    auto deltaMA = 10.0;
    auto deltaMR = 0.5;
    auto thetaA = 50.0;
    auto thetaR = 100.0;

    auto v = vessel_t{};
    auto env = v.environment();

    auto DA = v("DA", 1);
    auto D_A = v("D_A", 0);
    auto DR = v("DR", 1);
    auto D_R = v("D_R", 0);
    auto MA = v("MA", 0);
    auto MR = v("MR", 0);
    auto A = v("A", 0);
    auto R = v("R", 0);
    auto C = v("C", 0);

    v(A + DA >>= D_A, gammaA);
    v(D_A >>= DA + A, thetaA);
    v(A + DR >>= D_R, gammaR);
    v(D_R >>= DR + A, thetaR);
    v(D_A >>= MA + D_A, alpha_A);
    v(DA >>= MA + DA, alphaA);
    v(D_R >>= MR + D_R, alpha_R);
    v(DR >>= MR + DR, alphaR);
    v(MA >>= MA + A, betaA);
    v(MR >>= MR + R, betaR);
    v(A + R >>= C, gammaC);
    v(C >>= R, deltaA);
    v(A >>= env, deltaA);
    v(R >>= env, deltaR);
    v(MA >>= env, deltaMA);
    v(MR >>= env, deltaMR);

    return v;
}