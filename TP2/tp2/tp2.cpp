#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

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
// Création d'une matrice pour la mise a l'echelle
Matrix4x4 creerMatriceEchelle(double sx, double sy, double sz) {
    Matrix4x4 mat;
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    mat.m[2][2] = sz;
    return mat;
}
//les 3 prochaines fonction servent a appliquer une rotation et il y a une fonction par axe de rotation
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

// Fonction pour appliquer une projection en perspective
Vector3 FaireProjectionPerspective(const Vector3& point, float d) {
    // Vérifier que z ≠ 0 pour éviter la division par zéro
    if (point.z == 0.0f) {
        std::cerr << "Erreur : La projection n'est pas définie pour z = 0." << std::endl;
        return point;
    }

    // Appliquer la transformation de projection
    float x_prime = (d * point.x) / point.z;
    float y_prime = (d * point.y) / point.z;
    float z_prime = d; // Par convention
    return {x_prime, y_prime, z_prime};
}
// Matrice de réflexion
Matrix4x4 creerMatriceReflexion(bool x, bool y, bool z) {
    Matrix4x4 mat;
    if (x) mat.m[0][0] = -1;
    if (y) mat.m[1][1] = -1;
    if (z) mat.m[2][2] = -1;
    return mat;
}

bool saisirValeur(double &valeur) {
    cin >> valeur;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entree invalide, veuillez entrer un nombre valide.\n";
        return false;
    }
    return true;
}

int saisirEntier(int min, int max) {
    int valeur;
    while (true) {
        cin >> valeur;
        if (cin.fail() || valeur < min || valeur > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entree invalide, veuillez entrer un nombre entre " << min << " et " << max << ".\n";
        } else {
            return valeur;
        }
    }
}

// Fonction pour exécuter les tests unitaires
void executerTests() {
    cout << "Début des tests unitaires...\n";

    // Test de la translation
    Vector3 point = {1, 1, 1};
    Matrix4x4 translation = creerMatriceTranslation(2, 3, 4);
    Vector3 result = translation.appliquer(point);
    assert(result.x == 3 && result.y == 4 && result.z == 5);
    cout << "Test Translation réussi !\n";

    // Test de mise à l'échelle
    Matrix4x4 echelle = creerMatriceEchelle(2, 2, 2);
    result = echelle.appliquer(point);
    assert(result.x == 2 && result.y == 2 && result.z == 2);
    cout << "Test Mise à l'échelle réussi !\n";

    // Test de rotation X de 90°
    Matrix4x4 rotationX = creerMatriceRotationX(90);
    result = rotationX.appliquer(point);
    assert(abs(result.x - 1) < 1e-6 && abs(result.y - (-1)) < 1e-6 && abs(result.z - 1) < 1e-6);
    cout << "Test Rotation X réussi !\n";

    // Test de rotation Y de 90°
    Matrix4x4 rotationY = creerMatriceRotationY(90);
    result = rotationY.appliquer(point);
    assert(abs(result.x - 1) < 1e-6 && abs(result.y - 1) < 1e-6 && abs(result.z - (-1)) < 1e-6);
    cout << "Test Rotation Y réussi !\n";

    // Test de rotation Z de 90°
    Matrix4x4 rotationZ = creerMatriceRotationZ(90);
    result = rotationZ.appliquer(point);
    assert(abs(result.x - (-1)) < 1e-6 && abs(result.y - 1) < 1e-6 && abs(result.z - 1) < 1e-6);
    cout << "Test Rotation Z réussi !\n";

    // Test de cisaillement
    Matrix4x4 cisaillement = creerMatriceCisaillement(1, 0, 0, 1, 0, 0);
    result = cisaillement.appliquer(point);
    assert(abs(result.x - (1 + 1 * 1)) < 1e-6 && abs(result.y - (1 + 1 * 1)) < 1e-6 && abs(result.z - 1) < 1e-6);
    cout << "Test Cisaillement réussi !\n";

    // Test de réflexion par rapport à l'axe X
    Matrix4x4 reflexionX = creerMatriceReflexion(true, false, false);
    result = reflexionX.appliquer(point);
    assert(result.x == -1 && result.y == 1 && result.z == 1);
    cout << "Test Réflexion X réussi !\n";

    // Test de projection perspective
    Vector3 projected = FaireProjectionPerspective(point, 2);
    assert(abs(projected.x - 2.0 / 1) < 1e-6 && abs(projected.y - 2.0 / 1) < 1e-6 && projected.z == 2);
    cout << "Test Projection Perspective réussi !\n";

    cout << "Tous les tests unitaires sont passés avec succès !\n";
}



// Fonction principale
int main() {
    Vector3 point;
    cout << "Entrez les coordonnees du point (x y z) : ";
    while (!saisirValeur(point.x) || !saisirValeur(point.y) || !saisirValeur(point.z));

    Matrix4x4 transformationTotale;

    while (true) {
        cout << "\nChoisissez une transformation :\n";
        cout << "1. Translation\n";
        cout << "2. Mise a l echelle\n";
        cout << "3. Rotation (X, Y, Z)\n";
        cout << "4. Cisaillement\n";
        cout << "5. Projection\n";
        cout << "6. Reflexion\n";
        cout << "7. Tests Unitaires\n";
        cout << "8. Quitter\n";
        int choix = saisirEntier(1, 8);


        if (choix == 1) {
            double tx, ty, tz;
            cout << "Entrez les valeurs de translation (tx ty tz) : ";
            while (!saisirValeur(tx) || !saisirValeur(ty) || !saisirValeur(tz));
            transformationTotale = transformationTotale * creerMatriceTranslation(tx, ty, tz);
            Vector3 result = transformationTotale.appliquer(point);
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 2) {
            double sx, sy, sz;
            cout << "Entrez les valeurs d echelle (sx sy sz) : ";
            while (!saisirValeur(sx) || !saisirValeur(sy) || !saisirValeur(sz));
            transformationTotale = transformationTotale * creerMatriceEchelle(sx, sy, sz);
            Vector3 result = transformationTotale.appliquer(point);
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 3) {
            cout << "Choisissez l axe de rotation : X (1), Y (2), Z (3) : ";
            int axe = saisirEntier(1, 3);
            double angle;
            cout << "Entrez l angle en degres : ";
            while (!saisirValeur(angle));

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
            while (!saisirValeur(kxy) || !saisirValeur(kxz) || !saisirValeur(kyx) || !saisirValeur(kyz) || !saisirValeur(kzx) || !saisirValeur(kzy));
            Matrix4x4 C = creerMatriceCisaillement(kxy, kxz, kyx, kyz, kzx, kzy);
            transformationTotale = transformationTotale * C;
            Vector3 result = transformationTotale.appliquer(point);
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 5) {  // Projection
            double f;
            cout << "Entrez la nouvelle coordonee de z : ";
            while (!saisirValeur(f));
            Vector3 result = FaireProjectionPerspective(point, f);
            //ne mets pas a jour point je ne sais pas pourquoi
            //ou alors c'est qu'il faut mettre a jour transformation totale
            point = result;
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 6) {  // Réflexion
            bool rx, ry, rz;
            cout << "Reflechir par rapport a X ? (1: Oui, 0: Non) : ";
            rx = saisirEntier(0, 1);
            cout << "Reflechir par rapport a Y ? (1: Oui, 0: Non) : ";
            ry = saisirEntier(0, 1);
            cout << "Reflechir par rapport a Z ? (1: Oui, 0: Non) : ";
            rz = saisirEntier(0, 1);

            transformationTotale = transformationTotale * creerMatriceReflexion(rx, ry, rz);
            Vector3 result = transformationTotale.appliquer(point);
            cout << "Resultat apres transformation : ";
            result.afficher();
        }
        else if (choix == 7) {
            executerTests();
        }
        else if (choix == 8) {
            break;
        }
        else {
            cout << "Choix invalide !\n";
        }
    }

    return 0;
}
