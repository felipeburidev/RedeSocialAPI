package HotelAPI.model;

import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Entity
@Data
@NoArgsConstructor
@AllArgsConstructor
@Table(name = "stories")
public class Stories {

    @Id
    @Column(name = "id_Stories")
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int idStories;

    @Column(name = "conteudo")
    private String conteudo;

    @ManyToOne
    @JoinColumn(name = "id_usuario")
    private Usuario usuario;
}
