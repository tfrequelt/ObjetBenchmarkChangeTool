#define _USE_MATH_DEFINES

#include <iostream>
#include <sstream>
#include <cmath>

// Classe pour représenter un vecteur 3D
class Vec3 {
public:
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 transformByMatrix(const float matrix[3][3]) const {
        return Vec3(
            x * matrix[0][0] + y * matrix[0][1] + z * matrix[0][2],
            x * matrix[1][0] + y * matrix[1][1] + z * matrix[1][2],
            x * matrix[2][0] + y * matrix[2][1] + z * matrix[2][2]
        );
    }
};

class Transform {
public:
    Vec3 translation;
    float rotationMatrix[3][3];

    Transform(const Vec3& translation, float rotationAngleDegrees) {
        this->translation = translation;
        setRotationMatrix(rotationAngleDegrees);
    }

    void setRotationMatrix(float angleDegrees) {
        float angleRadians = angleDegrees * M_PI / 180.0f;
        float cosTheta = cos(angleRadians);
        float sinTheta = sin(angleRadians);

        rotationMatrix[0][0] = cosTheta; rotationMatrix[0][1] = 0; rotationMatrix[0][2] = sinTheta;
        rotationMatrix[1][0] = 0;        rotationMatrix[1][1] = 1; rotationMatrix[1][2] = 0;
        rotationMatrix[2][0] = -sinTheta; rotationMatrix[2][1] = 0; rotationMatrix[2][2] = cosTheta;
    }

    Vec3 apply(const Vec3& vector) const {
        Vec3 rotated = vector.transformByMatrix(rotationMatrix);
        return rotated + translation;
    }
};

int main() {
    while (true) {
        // Saisie de la translation
        std::cout << "Entrez les coordonnees de translation (x y z) ou appuyez sur Entrer pour utiliser les valeurs par defaut (-3 5 -1) : ";
        std::string translationInput;
        std::getline(std::cin, translationInput);
        Vec3 translation;
        if (translationInput.empty()) {
            translation = Vec3(-3, 5, -1); // Valeurs par défaut
        } else {
            std::istringstream iss(translationInput);
            float x, y, z;
            iss >> x >> y >> z;
            translation = Vec3(x, y, z);
        }
        
        bool angleValid = false;
        float rotationAngle = 72.0f; // Valeur par défaut
        while (!angleValid)
        {
            // Saisie de l'angle de rotation
            std::cout << "Entrez l'angle de rotation en degres ou appuyez sur Entrer pour utiliser la valeur par defaut (72) : ";
            std::string rotationInput;
            std::getline(std::cin, rotationInput);
            if (rotationInput.empty()) {
                break;
            }
            try
            {
                rotationAngle = std::stof(rotationInput);
                angleValid = true;
            }catch (const std::invalid_argument& e)
            {
                std::cout << "Veuillez rentrer un angle valide. \n ";
            }
        }

        // Création de la transformation
        Transform transform(translation, rotationAngle);

        // Demande des coordonnées du vecteur à transformer
        std::cout << "Entrez les coordonnees du vecteur dans le repere objet (x y z) : ";
        float x, y, z;
        std::cin >> x >> y >> z;
        while (std::cin.fail())
        {
            std::cin.clear(); // Réinitialise le flag d'erreur
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Vide le buffer
            std::cout << "Veuillez entrer un vecteur correct. \n Entrez les coordonnees du vecteur dans le repere objet (x y z) : ";
            std::cin >> x >> y >> z;
        }
        Vec3 objectVector(x, y, z);
        std::cin.ignore(); // Nettoyer le buffer d'entrée

        // Transformation du vecteur
        Vec3 worldVector = transform.apply(objectVector);

        // Affichage du résultat
        std::cout << "Le vecteur dans le repere universel est : ";
        worldVector.print();

        // Demander si l'utilisateur veut continuer
        char choice;
        std::cout << "Voulez-vous transformer un autre vecteur ? (o/n) : ";
        std::cin >> choice;
        std::cin.ignore(); // Nettoyer le buffer d'entrée

        if (choice == 'n' || choice == 'N') {
            std::cout << "Fin du programme." << std::endl;
            break;
        }
    }

    return 0;
}
