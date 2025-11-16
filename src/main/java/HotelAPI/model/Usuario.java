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
    private int id_usuario;

    @Column(name = "nome")
    private String nome;

    @Column(name = "email")
    private String email;

    @Column(name = "senha")
    private String senha;

    @Column(name = "data_nasc")
    private LocalDate dataNasc;

    @Column(name = "foto")
    private String foto;
    @Column(name = "bio")
    private String bio;         // descrição do usuário

    @Column(columnDefinition = "TEXT")
    private String interesses;  // lista de interesses ou JSON

    @Column(name = "privacidade")
    private String privacidade; // público, privado, amigos etc.


}
