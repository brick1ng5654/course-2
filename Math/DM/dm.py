# def modular_exponentiation(base, exp, mod):
#     result = 1
#     base = base % mod
#     while exp > 0:
#         if exp % 2 == 1:
#             result = (result * base) % mod
#         exp = exp >> 1
#         base = (base * base) % mod
#     return result

# def rsa_decrypt(ciphertext, d, m):
#     plaintext = []
#     for c in ciphertext:
#         plaintext.append(modular_exponentiation(c, d, m))
#     return plaintext

# # Constants
# e = 11
# p = 7
# q = 19
# m = p * q
# phi_m = (p - 1) * (q - 1)
# d = 59  # Found earlier

# ciphertext = [124, 37, 123, 70]
# plaintext = rsa_decrypt(ciphertext, d, m)

# # Map numbers to letters in the Russian alphabet
# alphabet = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
# def numbers_to_text(numbers):
#     return "".join(alphabet[num - 2] for num in numbers)

# # Decode
# decoded_message = numbers_to_text(plaintext)
# print("Decoded message:", decoded_message)


number = (70**59)%133
print(number)