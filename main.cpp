#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

double evaluate(
    double x,
    double y,
    double coe_x2,
    double coe_x1,
    double coe_y2,
    double coe_y1,
    double coe_xy,
    double cnst
)
{
    return
        coe_x2 * x * x +
        coe_x1 * x +
        coe_y2 * y * y +
        coe_y1 * y +
        coe_xy * x * y +
        cnst;
}

double calcScore(double value, double target, double sigma)
{
    double diff = value - target;
    return exp(-(diff * diff) / (2.0 * sigma * sigma));
}

int main()
{
    double coe_x2, coe_x1, coe_y2, coe_y1, coe_xy, cnst;
    double threshold;
    double xmin, xmax, ymin, ymax, xstep, ystep;
    double target, sigma;

    cout << "=== SBPE v0.1 ===" << endl;

    cout << "Input coefficients for:" << endl;
    cout << "f(x, y) = ax^2 + bx + cy^2 + dy + exy + f" << endl;

    cout << "a (x^2): ";
    cin >> coe_x2;

    cout << "b (x): ";
    cin >> coe_x1;

    cout << "c (y^2): ";
    cin >> coe_y2;

    cout << "d (y): ";
    cin >> coe_y1;

    cout << "e (xy): ";
    cin >> coe_xy;

    cout << "f (constant): ";
    cin >> cnst;

    cout << "\nTarget value: ";
    cin >> target;

    cout << "Sigma: ";
    cin >> sigma;

    cout << "\nThreshold (0.0 ~ 1.0): ";
    cin >> threshold;

    cout << "\nInput x range and step" << endl;
    cout << "xmin: ";
    cin >> xmin;

    cout << "xmax: ";
    cin >> xmax;

    cout << "xstep: ";
    cin >> xstep;

    cout << "\nInput y range and step" << endl;
    cout << "ymin: ";
    cin >> ymin;

    cout << "ymax: ";
    cin >> ymax;

    cout << "ystep: ";
    cin >> ystep;

    vector<vector<double>> values;

    double maxRawValue = -1e9;
    double minRawValue = 1e9;

    for (double y = ymin; y <= ymax; y += ystep)
    {
        vector<double> row;

        for (double x = xmin; x <= xmax; x += xstep)
        {
            double value = evaluate(
                x,
                y,
                coe_x2,
                coe_x1,
                coe_y2,
                coe_y1,
                coe_xy,
                cnst
            );

            maxRawValue = max(maxRawValue, value);
            minRawValue = min(minRawValue, value);

            row.push_back(value);
        }

        values.push_back(row);
    }

    cout << fixed << setprecision(2);

    cout << "\n=== SCORE MAP ===\n" << endl;

    cout << setw(8) << "y\\x";

    for (double x = xmin; x <= xmax; x += xstep)
    {
        cout << setw(6) << x;
    }

    cout << endl;

    int feasibleCount = 0;
    int totalCount = 0;

    double bestScore = -1.0;
    double bestX = xmin;
    double bestY = ymin;
    double bestRawAtBestScore = 0.0;

    double currentY = ymin;

    for (auto& row : values)
    {
        cout << setw(8) << currentY;

        double currentX = xmin;

        for (double value : row)
        {
            double score = calcScore(value, target, sigma);

            if (score >= threshold)
            {
                feasibleCount++;
            }

            if (score > bestScore)
            {
                bestScore = score;
                bestX = currentX;
                bestY = currentY;
                bestRawAtBestScore = value;
            }

            totalCount++;

            cout << setw(6) << score;

            currentX += xstep;
        }

        cout << endl;

        currentY += ystep;
    }

    cout << "\n=== SUMMARY ===\n" << endl;

    cout << "Target value         : " << target << endl;
    cout << "Sigma                : " << sigma << endl;
    cout << "Threshold            : " << threshold << endl;
    cout << "Best score           : " << bestScore << endl;
    cout << "Best point           : (" << bestX << ", " << bestY << ")" << endl;
    cout << "Best point raw value : " << bestRawAtBestScore << endl;
    cout << "Max raw value        : " << maxRawValue << endl;
    cout << "Min raw value        : " << minRawValue << endl;
    cout << "Feasible count       : " << feasibleCount << " / " << totalCount << endl;
    cout << "Feasible ratio       : "
         << 100.0 * feasibleCount / totalCount
         << "%" << endl;

    return 0;
}