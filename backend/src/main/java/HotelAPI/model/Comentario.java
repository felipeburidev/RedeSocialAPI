package HotelAPI.model;

import jakarta.persistence.*;
import lombok.*;
import java.time.LocalDateTime;

@Entity
@NoArgsConstructor
@AllArgsConstructor
@Data
@Table(name = "comentario")
public class Comentario {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id_comentario")
    private Integer idComentario; // Mude de Long para Integer (SERIAL = INT no PostgreSQL)

    @Column(name = "conteudo", nullable = false, columnDefinition = "TEXT")
    private String conteudo;

    @Column(name = "data_comentario")
    private LocalDateTime dataComentario; // Adicione este campo

    @ManyToOne
    @JoinColumn(name = "id_postagem", nullable = false)
    private Postagem postagem;

    @ManyToOne
    @JoinColumn(name = "id_usuario", nullable = false)
    private Usuario usuario;
}