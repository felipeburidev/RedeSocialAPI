package HotelAPI.model;

import jakarta.persistence.*;
import lombok.*;
import java.time.LocalDate;

@Entity
@NoArgsConstructor
@AllArgsConstructor
@Data
@Table(name = "usuario")
public class Usuario {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id_usuario")
    private Integer idUsuario;  // Mude para Integer e camelCase

    @Column(name = "nome", nullable = false, length = 100)
    private String nome;

    @Column(name = "email", nullable = false, unique = true, length = 100)
    private String email;

    @Column(name = "senha", nullable = false, length = 255)
    private String senha;

    @Column(name = "data_nasc")
    private LocalDate dataNasc;
}