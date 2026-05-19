; =====================================================================================
; Archivo:		rand_gen.inc
; Decripción:	Subrutina para llenar el vector con números pseudoaleatorios (LFSR)
; =====================================================================================

gen_vector:
	lds SEMILLA, SEMILLA_M;			Recupero el último estado de la semilla

	ldi ZH, high(VECTOR);			Puntero Z al inicio del vector en SRAM
	ldi ZL, low(VECTOR)

	ldi CONT_RAND, LARGO_VECTOR;	Inicializo la cantidad de bytes a generar
	
load:
	mov REG_AUX, SEMILLA
	andi REG_AUX, 0xB8;				Máscara para bits 7, 5, 4 y 3 (Polinomio LFSR)

loop_xor:
	; Casacada de desplazamientos (XORs)
	mov REG_TEMP, REG_AUX
	lsr REG_TEMP
	eor REG_AUX, REG_TEMP;			bit 4 XOR bit 3
	
	mov REG_TEMP, REG_AUX
	lsr REG_TEMP
	lsr REG_TEMP
	eor REG_AUX, REG_TEMP;			bit 5 XOR (bit 4 XOR bit 3)
	
	mov REG_TEMP, REG_AUX
	lsr REG_TEMP
	lsr	REG_TEMP
	lsr REG_TEMP
	lsr REG_TEMP
	eor REG_AUX, REG_TEMP;			bit 7 XOR (bit 5 XOR (bit 4 XOR bit 3))

	; Inyecto el resultado en la semilla
	lsr SEMILLA
	sbrc REG_AUX, 3
	ori SEMILLA, 0x80;				Si el bit resultante es 1, pongo en alto el MSB

	st Z+, SEMILLA;					Guardo el byte generado
	dec CONT_RAND;					Decremento el contador de bytes
	brne load

	sts semilla_m, SEMILLA;			Guardo la última semilla calculada

	ret;							Retorno de la subrutina
