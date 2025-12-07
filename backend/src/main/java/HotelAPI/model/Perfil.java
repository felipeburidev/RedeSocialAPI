package HotelAPI.model;

import jakarta.persistence.*;
import lombok.*;

@Entity
@Data
@NoArgsConstructor
@AllArgsConstructor
@Table(name = "perfil")
public class Perfil {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id_perfil")
    private int id_perfil;

    @Column(name = "nome")
    private String nome;

    @Column(name = "descricao")
    private String descricao;
}
