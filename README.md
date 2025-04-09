# TP_microControleurs_ANOT_BEAUV

### Voici le rapport du TP Systèmes à microcontrôleurs 


## Fait par :

| Claire ANOT-DELCOURT  | claire.anot-delcourt@ensea.fr  | Antoine BEAUVARLET    | antoine.beauvarlet@ensea.fr    |
| --------------------- | ------------------------------ | --------------------- | ------------------------------ |

## Encadré par :

| Nicolas PAPAZOGLOU    | nicolas.papazoglou@ensea.fr    |
| --------------------- | ------------------------------ |

# 1 Démarrage
1. Créez un projet pour la carte NUCLEO_L476RG. Initialisez les périphériques
avec leur mode par défaut, mais n’activez pas la BSP.


2. Testez la LED LD2.
#
```
int main(void) 
{  
    HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();

    while (1) {     
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);  
		HAL_Delay(1000);
    }  
}  
```
3. et 4. Testez l’USART2 connecté à la STLink interne.  Débrouillez-vous pour que la fonction printf fonctionne
```
//Q4 :
int __io_putchar(int chr)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&chr, 1, HAL_MAX_DELAY);

	return chr;
}

int main(void)
{
//  Reset of all peripherals, Initializes the Flash interface and the Systick.
	HAL_Init();

//  Configure the system clock
	SystemClock_Config();



//  Initialize all configured peripherals
	MX_GPIO_Init();
	MX_USART2_UART_Init();

	while (1)
	{
//  Q3 :
//      HAL_UART_Transmit(&huart2, "Hello world\r\n", 13, 100); 

//  Q4 :
		printf("Hello world\r\n"); 

		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(1000);
	}
}


```

# 2 Le GPIO Expander et le VU-Metre
## Configuration
1. Quelle est la référence du GPIO Expander ? Vous aurez besoin de sa datasheet, téléchargez-la.

> référence : MCP23S17

>https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP23017-Data-Sheet-DS20001952.pdf

2. Sur le STM32, quel SPI est utilisé ?
>   High-Speed SPI Interface (MCP23S17) :
>- 10 MHz (maximum)
>- SPI1, SPI2 ou SPI3
>- En remontant le circuit on arrive a soit SPI1 soit SPI3 
>- Dans Cube IDE SPI1 est bloqué donc c'est `SPI3` qui est utilisé




3. Quels sont les paramètres à configurer dans STM32CubeIDE ?

| Nom | Valeur |
| ------ | ------- |
| Data Size| 8 Bits|
| Clock Polarity | Low|
|Clock Phase| 1 Edge|
|Baud Rate Prescaler | 40Mbit/s|
|First Bit | MSB First|


## Tests
![alt text](image-8.png)
1. Faites clignoter une ou plusieurs LED et Pour toutes les tester, vous pouvez faire un chenillard (par exemple).

![Texte alternatif](chenillard.gif)
## Driver

ON EST ICI - Piloter les LEDs avec TeraTerm (driver)

1. Écrivez un driver pour piloter les LED. Utilisez une structure.

2. Écrivez une fonction shell permettant d’allumer ou d’éteindre n’importe
quelle LED.
```
void MCP23S17_ClignoterLED(uint8_t num_LED, uint32_t delay) {
    uint8_t port_A_data = 0xFF; // Toutes les LEDs éteintes par défaut
    uint8_t port_B_data = 0xFF;

    if (num_LED < 1 || num_LED > 16) {
        // Numéro de LED invalide
        return; // Ou gérer l'erreur d'une autre manière
    }

    switch (num_LED) {
        case 1:  port_A_data &= ~(1 << 0); break;
        case 2:  port_A_data &= ~(1 << 1); break;
        case 3:  port_A_data &= ~(1 << 2); break;
        case 4:  port_A_data &= ~(1 << 3); break;
        case 5:  port_A_data &= ~(1 << 4); break;
        case 6:  port_A_data &= ~(1 << 5); break;
        case 7:  port_A_data &= ~(1 << 6); break;
        case 8:  port_A_data &= ~(1 << 7); break;
        case 9:  port_B_data &= ~(1 << 0); break;
        case 10: port_B_data &= ~(1 << 1); break;
        case 11: port_B_data &= ~(1 << 2); break;
        case 12: port_B_data &= ~(1 << 3); break;
        case 13: port_B_data &= ~(1 << 4); break;
        case 14: port_B_data &= ~(1 << 5); break;
        case 15: port_B_data &= ~(1 << 6); break;
        case 16: port_B_data &= ~(1 << 7); break;
    }

    // Allumer la LED spécifiée
    MCP23S17_WriteRegister(0x12, ~port_A_data);
    MCP23S17_WriteRegister(0x13, ~port_B_data);
    HAL_Delay(delay);

    // Eteindre la LED
    MCP23S17_WriteRegister(0x12, 0xFF);
    MCP23S17_WriteRegister(0x13, 0xFF);
    HAL_Delay(delay);
}
```
Dans le main :
```
while (1) {
        // Faire clignoter la LED 1 à 500 ms
        MCP23S17_ClignoterLED(1, 500);
}
```
# 3 Le CODEC Audio SGTL5000
## Configuration préalables
Le CODEC a besoin de deux protocoles de communication :
>- L’I2C pour la configuration,
>- L’I2S pour le transfert des échantillons audio.
  
Les configurations suivantes sont à faire sur le logiciel STM32CubeIDE dans la partie graphique CubeMX. Le protocole I2S est géré par le périphérique SAI (Serial Audio Interface).

1. Quelles pins sont utilisées pour l’I2C ? À quel I2C cela correspond dans leSTM32 ?
![alt text](image-7.png)

Broches I2C sur le STM32 NUCLEO-L476RG : 

Le STM32L476RG dispose de plusieurs périphériques I2C (I2C1, I2C2, etc.).
Les broches utilisées pour l'I2C dépendent du périphérique I2C sélectionné.
Typiquement, pour l'I2C1, on retrouve:
>- PB8 : I2C1_SCL (Serial Clock)
>- PB9 : I2C1_SDA (Serial Data)

2. Activez l’I2C correspondant, laissez la configuration par défaut.
   
3. Configurez le SAI2 :
>- SAI A : Master with Master Clock Out,
>- Cochez I2S/PCM protocol,
>- SAI B : Synchronous Slave,
>- Cochez I2S/PCM protocol.
  
4. Si nécessaire, déplacez les signaux sur les bonnes broches. Vous pouvez déplacer une broche avec un [Ctrl+Clic Gauche]. Les signaux du SAI doivent être connectés au broches suivantes :
   
>| Broches | Signaux   | 
>| ----- | ----------- | 
>| PB12  | SAI2_FS_A   |
>| PB13  | SAI2_SCK_A  |
>| PB14  | SAI2_MCLK_A |
>| PB15  | SAI2_SD_A   |
>| PC12  | SAI2_SD_B   |

5. Dans l’onglet Clock Configuration, configurez PLLSAI1 pour obtenir la fréquence To SAI2 à 12.235294 MHz
   
6. Configurez les blocs SAI A et SAI B
7. Activez les interruptions.
8. Configurez le DMA pour le SAI A et le SAI B. Activez le mode circulaire.
9. Avant de passer à la suite, il est nécessaire d’activer l’horloge MCLK pour que le CODEC fonctionne. Pour cela, dans la fonction main(), après les initialisations, ajoutez la ligne suivante :
>__HAL_SAI_ENABLE(&hsai_BlockA2);

Note Sans cette ligne, l’I2C ne fonctionne pas, parce que le CODEC ne
reçoit pas d’horloge !
## 3.2 Configuration du CODEC par l’I2C
1. À l’aide d’un oscilloscope, vérifiez la présence d’une horloge sur le signal MCLK.
   On observe bien un retour sur la broche MCLK
   
2. À l’aide de la fonction HAL_I2C_Mem_Read(), récupérez la valeur du registre CHIP_ID (addresse 0x0000). L’adresse I2C du CODEC est 0x14.
   
3. Observez les trames I2C à l’oscilloscope.
   
4. Montrez à l’enseignant.
   
5. Cherchez dans la documentation du SGTL5000 la valeur à assigner aux registres suivants :
>- CHIP_ANA_POWER
>- CHIP_LINREG_CTRL
>- CHIP_REF_CTRL
>- CHIP_LINE_OUT_CTRL
>- CHIP_SHORT_CTRL
>- CHIP_ANA_CTRL
>- CHIP_ANA_POWER
>- CHIP_DIG_POWER
>- CHIP_LINE_OUT_VOL
>- CHIP_CLK_CTRL
>- CHIP_I2S_CTRL
>- CHIP_ADCDAC_CTRL
>- CHIP_DAC_VOL


## CONFIGURATION DU CHIP

Toutes les sorties (LINEOUT, HP_OUT, I2S_OUT) sont mises en sourdine par défaut à la mise sous tension. Pour éviter tout bruit parasite (pops/clicks), les sorties doivent rester en sourdine pendant ces étapes de configuration du chip. Référez-vous au Contrôle du Volume pour le contrôle du volume et de la sourdine.

## Initialisation

### Mise sous tension du chip et configurations d'alimentation

Une fois les alimentations du chip activées, la séquence d'initialisation suivante doit être suivie. Veuillez noter que certaines étapes peuvent être facultatives ou que des valeurs différentes peuvent devoir être écrites en fonction de la tension d'alimentation utilisée et de la configuration souhaitée. La séquence d'initialisation ci-dessous suppose VDDIO = 3,3 V et VDDA = 1,8 V.
```
//--------------- Configuration de l'alimentation ----------------
// NOTE : Ces 2 appels d'écriture suivants sont nécessaires UNIQUEMENT si VDDD est
// piloté en interne par le chip
// Configurer le niveau VDDD à 1,2 V (bits 3:0)
Write CHIP_LINREG_CTRL 0x0008
// Mettre sous tension le régulateur linéaire interne (Définir le bit 9)
Write CHIP_ANA_POWER 0x7260

// NOTE : Cet appel d'écriture suivant est nécessaire UNIQUEMENT si VDDD est
// piloté en externe
// Désactiver les alimentations de démarrage pour économiser de l'énergie (Effacer les bits 12 et 13)
Write CHIP_ANA_POWER 0x4260

// NOTE : Les appels d'écriture suivants sont nécessaires uniquement si les alimentations VDDA et
// VDDIO sont inférieures à 3,1 V.
// Activer l'oscillateur interne pour la pompe de charge (Définir le bit 11)
Write CHIP_CLK_TOP_CTRL 0x0800
// Activer la pompe de charge (Définir le bit 11)
Write CHIP_ANA_POWER 0x4A60

// NOTE : L'appel de modification suivant est uniquement nécessaire si VDDA et
// VDDIO sont supérieures à 3,1 V
// Configurer la pompe de charge pour utiliser le rail VDDIO (définir le bit 5 et le bit 6)
Write CHIP_LINREG_CTRL 0x006C
```
```
//---- Tension de référence et configuration du courant de polarisation ----
// NOTE : La valeur écrite dans les 2 appels d'écriture suivants dépend
// de la valeur de la tension VDDA.
// Définir la tension de référence de masse, ADC, DAC (bits 8:4). La valeur doit
// être définie sur VDDA/2. Cet exemple suppose VDDA = 1,8 V.
VDDA/2 = 0,9 V.
// Le courant de polarisation doit être défini sur 50% de la valeur nominale (bits 3:1)
Write CHIP_REF_CTRL 0x004E
// Définir la tension de référence LINEOUT à VDDIO/2 (1,65 V) (bits 5:0)
// et le courant de polarisation (bits 11:8) à la valeur recommandée de 0,36 mA
// pour une charge de 10 kOhm avec une capacité de 1,0 nF
Write CHIP_LINE_OUT_CTRL 0x0322
```
```
//------------ Autres configurations de blocs analogiques --------------
// Configurer une vitesse de montée lente pour minimiser les bruits parasites (bit 0)
Write CHIP_REF_CTRL 0x004F
// Activer le mode de détection de court-circuit pour les canaux gauche/droite
// et central du casque et définir le niveau de déclenchement du courant de détection de court-circuit
// à 75 mA
Write CHIP_SHORT_CTRL 0x1106
// Activer la détection de passage par zéro si nécessaire pour HP_OUT (bit 5) et ADC (bit 1)
Write CHIP_ANA_CTRL 0x0133
```
//------------ Mise sous tension des entrées/sorties/blocs numériques ---------
// Mettre sous tension LINEOUT, HP, ADC, DAC
Write CHIP_ANA_POWER 0x6AFF
// Mettre sous tension les blocs numériques souhaités
// I2S_IN (bit 0), I2S_OUT (bit 1), DAP (bit 4), DAC (bit 5),
// ADC (bit 6) sont mis sous tension
Write CHIP_DIG_POWER 0x0073
```
//---------------- Définir le niveau de volume LINEOUT -------------------
// Définir le niveau de volume LINEOUT en fonction des valeurs de référence de tension (VAG)
// en utilisant cette formule
// Valeur = (int)(40*log(VAG_VAL/LO_VAGCNTRL) + 15)
// En supposant que VAG_VAL et LO_VAGCNTRL sont définis respectivement sur 0,9 V et 1,65 V, le
// volume LO gauche (bits 12:8) et le volume LO droit (bits 4:0) doivent être définis sur 5
Write CHIP_LINE_OUT_VOL 0x0505
```

### Horloge Système MCLK et Horloge d'Échantillonnage
```
// Configurer l'horloge SYS_FS à 48 kHz
// Configurer MCLK_FREQ à 256*Fs
Modify CHIP_CLK_CTRL->SYS_FS 0x0002 // bits 3:2
Modify CHIP_CLK_CTRL->MCLK_FREQ 0x0000 // bits 1:0
// Configurer les horloges I2S en mode maître
// NOTE : I2S LRCLK est la même que l'horloge d'échantillonnage du système
Modify CHIP_I2S_CTRL->MS 0x0001 // bit 7
```
### Configuration de la PLL

Ces étapes de programmation sont nécessaires uniquement lorsque la PLL est utilisée. Référez-vous à "Using the PLL - Asynchronous SYS_MCLK input" pour plus de détails sur quand utiliser la PLL.

Pour éviter tout bruit parasite (pops/clicks), les sorties doivent être mises en sourdine pendant ces étapes de configuration du chip. Référez-vous à "Volume Control" pour le contrôle du volume et de la sourdine.
```
// Mettre sous tension la PLL
Modify CHIP_ANA_POWER->PLL_POWERUP 0x0001 // bit 10
Modify CHIP_ANA_POWER->VCOAMP_POWERUP 0x0001 // bit 8
// NOTE : Cette étape est requise uniquement lorsque le SYS_MCLK externe
// est supérieur à 17 MHz. Dans ce cas, l'horloge SYS_MCLK externe
// doit être divisée par 2
Modify CHIP_CLK_TOP_CTRL->INPUT_FREQ_DIV2 0x0001 // bit 3
Sys_MCLK_Input_Freq = Sys_MCLK_Input_Freq/2;
// La fréquence de sortie de la PLL est différente en fonction de la fréquence d'échantillonnage utilisée.
if (Sys_Fs_Rate == 44.1 kHz)
 PLL_Output_Freq = 180.6336 MHz
else
 PLL_Output_Freq = 196.608 MHz
// Définir les diviseurs de la PLL
Int_Divisor = floor(PLL_Output_Freq/Sys_MCLK_Input_Freq)
Frac_Divisor = ((PLL_Output_Freq/Sys_MCLK_Input_Freq) - Int_Divisor)*2048
Modify CHIP_PLL_CTRL->INT_DIVISOR Int_Divisor // bits 15:11
Modify CHIP_PLL_CTRL->FRAC_DIVISOR Frac_Divisor // bits 10:0
```
### Routage des Entrées/Sorties

Pour éviter tout bruit parasite (pops/clicks), les sorties doivent être mises en sourdine pendant ces étapes de configuration du chip. Référez-vous à "Volume Control" pour le contrôle du volume et de la sourdine.

Quelques exemples de routage sont présentés ci-dessous :
```
// Exemple 1 : I2S_IN -> DAP -> DAC -> LINEOUT, HP_OUT
// Router I2S_IN vers DAP
Modify CHIP_SSS_CTRL->DAP_SELECT 0x0001 // bits 7:6
// Router DAP vers DAC
Modify CHIP_SSS_CTRL->DAC_SELECT 0x0003 // bits 5:4
// Sélectionner DAC comme entrée pour HP_OUT
Modify CHIP_ANA_CTRL->SELECT_HP 0x0000 // bit 6
// Exemple 2 : MIC_IN -> ADC -> I2S_OUT
// Définir l'entrée ADC à MIC_IN
Modify CHIP_ANA_CTRL->SELECT_ADC 0x0000 // bit 2
// Router ADC vers I2S_OUT
Modify CHIP_SSS_CTRL->I2S_SELECT 0x0000 // bits 1:0
// Exemple 3 : LINEIN -> HP_OUT
// Sélectionner LINEIN comme entrée pour HP_OUT
Modify CHIP_ANA_CTRL->SELECT_HP 0x0001 // bit 6
```







6. Créez une paire de fichier sgtl5000.c / sgtl5000.h

7. Dans le fichier sgtl5000.c, créez une fonction d’initialisation.

8. Dans cette fonction, écrivez le code permettant de configurer ces registres.

# Fichier sgtl5000.h
```
#ifndef SGTL5000_H_
#define SGTL5000_H_
#include "stm32l4xx_hal.h"

// Adresses des registres du SGTL5000
#define CHIP_LINREG_CTRL 0x0006
#define CHIP_ANA_POWER 0x0002
#define CHIP_CLK_TOP_CTRL 0x0004
#define CHIP_REF_CTRL 0x000A
#define CHIP_LINE_OUT_CTRL 0x0010
#define CHIP_SHORT_CTRL 0x0014
#define CHIP_ANA_CTRL 0x0016
#define CHIP_DIG_POWER 0x003C
#define CHIP_LINE_OUT_VOL 0x0020
#define CHIP_CLK_CTRL 0x0000
#define CHIP_I2S_CTRL 0x003A
#define CHIP_PLL_CTRL 0x0008
#define CHIP_SSS_CTRL 0x002E

// Fonction d'initialisation
void SGTL5000_Init(I2C_HandleTypeDef *hi2c);

// Fonction d'écriture de registre
HAL_StatusTypeDef SGTL5000_WriteReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t data);

#endif /* SGTL5000_H_ */
```
# Fichier sgtl5000.c
```
#include "sgtl5000.h"
// Fonction d'écriture de registre
HAL_StatusTypeDef SGTL5000_WriteReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t data) {
    uint8_t tx_buffer[3];
    tx_buffer[0] = (reg << 1); // Adresse du registre + bit d'écriture (0)
    tx_buffer[1] = (data >> 8) & 0xFF; // Octet de poids fort
    tx_buffer[2] = data & 0xFF; // Octet de poids faible
    return HAL_I2C_Master_Transmit(hi2c, 0x0A << 1, tx_buffer, 3, HAL_MAX_DELAY); // adresse sgtl5000 = 0x0A
}

// Fonction d'initialisation
void SGTL5000_Init(I2C_HandleTypeDef *hi2c) {

    // Configuration de l'alimentation
    SGTL5000_WriteReg(hi2c, CHIP_LINREG_CTRL, 0x0008);
    SGTL5000_WriteReg(hi2c, CHIP_ANA_POWER, 0x7260);
    SGTL5000_WriteReg(hi2c, CHIP_CLK_TOP_CTRL, 0x0800);
    SGTL5000_WriteReg(hi2c, CHIP_ANA_POWER, 0x4A60);
    SGTL5000_WriteReg(hi2c, CHIP_LINREG_CTRL, 0x006C);

    // Tension de référence et configuration du courant de polarisation
    SGTL5000_WriteReg(hi2c, CHIP_REF_CTRL, 0x004E);
    SGTL5000_WriteReg(hi2c, CHIP_LINE_OUT_CTRL, 0x0322);

    // Autres configurations de blocs analogiques
    SGTL5000_WriteReg(hi2c, CHIP_REF_CTRL, 0x004F);
    SGTL5000_WriteReg(hi2c, CHIP_SHORT_CTRL, 0x1106);
    SGTL5000_WriteReg(hi2c, CHIP_ANA_CTRL, 0x0133);
    
	// Mise sous tension des entrées/sorties/blocs numériques
    SGTL5000_WriteReg(hi2c, CHIP_ANA_POWER, 0x6AFF);
    SGTL5000_WriteReg(hi2c, CHIP_DIG_POWER, 0x0073);
    
	// Définir le niveau de volume LINEOUT
    SGTL5000_WriteReg(hi2c, CHIP_LINE_OUT_VOL, 0x0505);
    
	// Horloge Système MCLK et Horloge d'Échantillonnage
    SGTL5000_WriteReg(hi2c, CHIP_CLK_CTRL, 0x0002);
    SGTL5000_WriteReg(hi2c, CHIP_CLK_CTRL, 0x0000);
    SGTL5000_WriteReg(hi2c, CHIP_I2S_CTRL, 0x0001);
    
	// Configuration de la PLL
    SGTL5000_WriteReg(hi2c, CHIP_ANA_POWER, 0x0400);
    SGTL5000_WriteReg(hi2c, CHIP_ANA_POWER, 0x0100);
    SGTL5000_WriteReg(hi2c, CHIP_CLK_TOP_CTRL, 0x0008);
    SGTL5000_WriteReg(hi2c, CHIP_PLL_CTRL, 0x6028);
    SGTL5000_WriteReg(hi2c, CHIP_PLL_CTRL, 0x07D0);
    
	// Routage des Entrées/Sorties
    SGTL5000_WriteReg(hi2c, CHIP_SSS_CTRL, 0x0001);
    SGTL5000_WriteReg(hi2c, CHIP_SSS_CTRL, 0x0003);
    SGTL5000_WriteReg(hi2c, CHIP_ANA_CTRL, 0x0000);
    SGTL5000_WriteReg(hi2c, CHIP_ANA_CTRL, 0x0000);
    SGTL5000_WriteReg(hi2c, CHIP_SSS_CTRL, 0x0000);
    SGTL5000_WriteReg(hi2c, CHIP_ANA_CTRL, 0x0040);
}
```
## 3.3 Signaux I2S
1. Démarrez la réception et la transmission sur l’I2S avec le SAI :
>- HAL_SAI_Receive_DMA()
>- HAL_SAI_Transmit_DMA()

2. Observez à l’oscilloscope les différents signaux d’horloge.

3. Montrez à l’enseignant.

## 3.4 Génération de signal audio
1. Générez un signal triangulaire.

2. Vérifier à l’oscilloscope, montrez à l’enseignant.

## 3.5 Bypass numérique
1. Écrivez le code permettant de lire les échantillons de l’ADC, et de les écrire sur le DAC.

2. Vérifier à l’oscilloscope, montrez à l’enseignant.

# 4 Visualisation
1. Écrivez le code permettant de visualiser le volume sonore sur les LED.
>https://en.wikipedia.org/wiki/VU_meter

2. Montrez à l’enseignant.

# 5 Filtre RC
![alt text](image-1.png)

![alt text](image-3.png)

![alt text](image-4.png)

![alt text](image-5.png)

![alt text](image-6.png)

4. Pour une fréquence d’échantillonnage de 48kHz, combien de cycles processeurs disposons-nous pour traiter chaque échantillon ?
5. Créez une paire de fichiers RCFilter.c / RCFilter.h

6. Créez la structure suivante dans RCFilter.h :
#
typedef struct {  
uint32_t coeff_A;  
uint32_t coeff_B;  
uint32_t coeff_D;  
uint16_t out_prev;  
} h_RC_filter_t;  
#
7. Écrivez les fonctions suivantes :
   
>// Calcule les coefficients A, B et D
>// Et les stocke dans la structure  
void RC_filter_init(h_RC_filter_t * h_RC_filter, uint16_t cutoff_frequency, int16_t sampling_frequency);
>// Implémente l'équation de récurrence
>// Faites attention au type des différentes variables  
uint16_t RC_filter_update(h_RC_filter_t * h_RC_filter, uint16_t input);

8. Ajoutez une fonction au Shell pour modifier la fréquence de coupure.

9. Faites valider par votre enseignant.

# 6 Programmation d’un effet audio

1. Programmez un effet audio de votre choix :
(a) Saturation/distortion  

(b) Tremolo  

(c) Filtre analogique  

(d) Delay (court...)  

(e) Chorus/Phaser/Flanger  

(f) Compresseur  

(g) Reverb  


