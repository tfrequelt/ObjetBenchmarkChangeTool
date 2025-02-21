#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Structure pour un vecteur 3D
struct Vector3 {
    double x, y, z;

    void afficher() const {
        cout << "(" << x << ", " << y << ", " << z << ")" << endl;
    }
};

// Classe pour une matrice 4x4
class Matrix4x4 {
public:
    double m[4][4];

    Matrix4x4() {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = (i == j) ? 1.0 : 0.0;
    }

    // Multiplier la matrice par un vecteur 3D
    Vector3 appliquer(const Vector3& v) const {
        Vector3 res;
        res.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        res.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        res.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        return res;
    }

    // Multiplier deux matrices entre elles
    Matrix4x4 operator*(const Matrix4x4& other) const {
        Matrix4x4 res;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                res.m[i][j] = 0;
                for (int k = 0; k < 4; ++k)
                    res.m[i][j] += m[i][k] * other.m[k][j];
            }
        return res;
    }
};

// Création des matrices de transformation
Matrix4x4 creerMatriceTranslation(double tx, double ty, double tz) {
    Matrix4x4 mat;
    mat.m[0][3] = tx;
    mat.m[1][3] = ty;
    mat.m[2][3] = tz;
    return mat;
}

Matrix4x4 creerMatriceEchelle(double sx, double sy, double sz) {
    Matrix4x4 mat;
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    mat.m[2][2] = sz;
    return mat;
}

Matrix4x4 creerMatriceRotationX(double angle) {
    Matrix4x4 mat;
    double rad = angle * M_PI / 180.0;
    mat.m[1][1] = cos(rad);
    mat.m[1][2] = -sin(rad);
    mat.m[2][1] = sin(rad);
    mat.m[2][2] = cos(rad);
    return mat;
}

Matrix4x4 creerMatriceRotationY(double angle) {
    Matrix4x4 mat;
    double rad = angle * M_PI / 180.0;
    mat.m[0][0] = cos(rad);
    mat.m[0][2] = sin(rad);
    mat.m[2][0] = -sin(rad);
    mat.m[2][2] = cos(rad);
    return mat;
}

Matrix4x4 creerMatriceRotationZ(double angle) {
    Matrix4x4 mat;
    double rad = angle * M_PI / 180.0;
    mat.m[0][0] = cos(rad);
    mat.m[0][1] = -sin(rad);
    mat.m[1][0] = sin(rad);
    mat.m[1][1] = cos(rad);
    return mat;
}

// Matrice de cisaillement
Matrix4x4 creerMatriceCisaillement(double kxy, double kxz, double kyx, double kyz, double kzx, double kzy) {
    Matrix4x4 mat;
    mat.m[0][1] = kxy;
    mat.m[0][2] = kxz;
    mat.m[1][0] = kyx;
    mat.m[1][2] = kyz;
    mat.m[2][0] = kzx;
    mat.m[2][1] = kzy;
    return mat;
}

// Matrice de projection perspective
Matrix4x4 creerMatriceProjection(double f) {
    Matrix4x4 mat;
    mat.m[3][2] = -1.0 / f;  // Perspective simple
    return mat;
}

// Matrice de réflexion
Matrix4x4 creerMatriceReflexion(bool x, bool y, bool z) {
    Matrix4x4 mat;
    if (x) mat.m[0][0] = -1;
    if (y) mat.m[1][1] = -1;
    if (z) mat.m[2][2] = -1;
    return mat;
}

// Fonction principale
int main() {
    Vector3 point;
    cout << "Entrez les coordonnees du point (x y z) : ";
    cin >> point.x >> point.y >> point.z;

    Matrix4x4 transformationTotale;

    while (true) {
        cout << "\nChoisissez une transformation :\n";
        cout << "1. Translation\n";
        cout << "2. Mise a l echelle\n";
        cout << "3. Rotation (X, Y, Z)\n";
        cout << "4. Cisaillement\n";
        cout << "5. Projection\n";
        cout << "6. Reflexion\n";
        cout << "7. Quitter\n";
        int choix;
        cin >> choix;

        if (choix == 1) {
            double tx, ty, tz;
            cout << "Entrez les valeurs de translation (tx ty tz) : ";
            cin >> tx >> ty >> tz;
            transformationTotale = transformationTotale * creerMatriceTranslation(tx, ty, tz);
            Vector3 result = transformationTotale.appliquer(point);
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 2) {
            double sx, sy, sz;
            cout << "Entrez les valeurs d echelle (sx sy sz) : ";
            cin >> sx >> sy >> sz;
            transformationTotale = transformationTotale * creerMatriceEchelle(sx, sy, sz);
            Vector3 result = transformationTotale.appliquer(point);
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 3) {
            cout << "Choisissez l axe de rotation : X (1), Y (2), Z (3) : ";
            int axe;
            cin >> axe;
            double angle;
            cout << "Entrez l angle en degres : ";
            cin >> angle;

            if (axe == 1)
                transformationTotale = transformationTotale * creerMatriceRotationX(angle);
            else if (axe == 2)
                transformationTotale = transformationTotale * creerMatriceRotationY(angle);
            else if (axe == 3)
                transformationTotale = transformationTotale * creerMatriceRotationZ(angle);
            Vector3 result = transformationTotale.appliquer(point);
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 4) {  // Cisaillement
            double kxy, kxz, kyx, kyz, kzx, kzy;
            cout << "Entrez les coefficients de cisaillement (kxy kxz kyx kyz kzx kzy) : ";
            cin >> kxy >> kxz >> kyx >> kyz >> kzx >> kzy;
            Matrix4x4 C = creerMatriceCisaillement(kxy, kxz, kyx, kyz, kzx, kzy);
            transformationTotale = transformationTotale * C;
            Vector3 result = transformationTotale.appliquer(point);
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 5) {  // Projection
            double f;
            cout << "Entrez la distance focale : ";
            cin >> f;
            Matrix4x4 P = creerMatriceProjection(f);
            transformationTotale = transformationTotale * P;
            Vector3 result = transformationTotale.appliquer(point);
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 6) {  // Réflexion
            bool rx, ry, rz;
            cout << "Reflexion par rapport a X ? (1 = Oui, 0 = Non) : ";
            cin >> rx;
            cout << "Reflexion par rapport a Y ? (1 = Oui, 0 = Non) : ";
            cin >> ry;
            cout << "Reflexion par rapport a Z ? (1 = Oui, 0 = Non) : ";
            cin >> rz;
            Matrix4x4 R = creerMatriceReflexion(rx, ry, rz);
            transformationTotale = transformationTotale * R;
            Vector3 result = transformationTotale.appliquer(point);
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 7) {
            break;
        }
        else {
            cout << "Choix invalide !\n";
        }
    }

    return 0;
}
