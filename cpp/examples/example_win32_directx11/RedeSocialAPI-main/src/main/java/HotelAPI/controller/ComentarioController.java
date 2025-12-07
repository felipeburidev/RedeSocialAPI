package HotelAPI.controller;

import HotelAPI.model.Comentario;
import HotelAPI.model.Postagem;
import HotelAPI.model.Usuario;
import HotelAPI.repository.ComentarioRepository;
import HotelAPI.repository.PostagemRepository;
import HotelAPI.repository.UsuarioRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import java.time.LocalDateTime;
import java.util.List;

@RestController
@RequestMapping("/comentarios")
public class ComentarioController {

    @Autowired
    private ComentarioRepository comentarioRepository;

    @Autowired
    private PostagemRepository postagemRepository;

    @Autowired
    private UsuarioRepository usuarioRepository;

    @PostMapping
    public Comentario criarComentario(
            @RequestParam Integer idPostagem,  // Mude de int para Integer
            @RequestParam Integer idUsuario,
            @RequestBody Comentario comentario) {

        Postagem postagem = postagemRepository.findById(idPostagem)
                .orElseThrow(() -> new RuntimeException("Postagem não encontrada"));

        Usuario usuario = usuarioRepository.findById(idUsuario)
                .orElseThrow(() -> new RuntimeException("Usuário não encontrado"));

        comentario.setPostagem(postagem);
        comentario.setUsuario(usuario);
        comentario.setDataComentario(LocalDateTime.now());  // Set a data atual

        return comentarioRepository.save(comentario);
    }

    @GetMapping
    public List<Comentario> listarTodos() {
        return comentarioRepository.findAll();
    }

    @GetMapping("/postagem/{idPostagem}")
    public List<Comentario> listarPorPostagem(@PathVariable Integer idPostagem) {
        return comentarioRepository.findByPostagem_IdPostagem(idPostagem);
    }

    @GetMapping("/{id}")
    public Comentario buscarPorId(@PathVariable Integer id) {  // Mude de Long para Integer
        return comentarioRepository.findById(id)
                .orElseThrow(() -> new RuntimeException("Comentário não encontrado"));
    }

    @PutMapping("/{id}")
    public Comentario atualizar(
            @PathVariable Integer id,  // Mude de Long para Integer
            @RequestBody Comentario comentarioAtualizado) {

        Comentario comentario = comentarioRepository.findById(id)
                .orElseThrow(() -> new RuntimeException("Comentário não encontrado"));

        comentario.setConteudo(comentarioAtualizado.getConteudo());
        // Não atualize a data aqui, mantenha a data original do comentário

        return comentarioRepository.save(comentario);
    }

    @DeleteMapping("/{id}")
    public void deletar(@PathVariable Integer id) {  // Mude de Long para Integer
        comentarioRepository.deleteById(id);
    }
}